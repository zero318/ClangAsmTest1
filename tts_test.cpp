#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <atomic>

#include <vector>
#include <string>
#include <map>

#include "../zero/custom_intrin.h"
#include "../zero/util.h"

#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <sapi.h>
//#include <sapi53.h>
#include <sphelper.h>

#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>

#include <math.h>
#include <time.h>

#include <speech.h>

// https://github.com/TETYYS/SAPI4

static std::vector<std::wstring> voice_names;

void parse_windows_messages() {
	if (MSG msg; GetMessage(&msg, (HWND)NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static std::map<std::string_view, const wchar_t*> voice5_lookup = {
	{ "anna", L"Microsoft Anna - English (United States)" },
	{ "Anna", L"Microsoft Anna - English (United States)" },
};

void print_voice_list() {
	for (auto& name : voice_names) {
		printf("%ls\n", name.c_str());
	}
}

template <typename T>
struct COMReleaser {
	T& backing;

	inline ~COMReleaser() {
		this->backing->Release();
	}
};

#define COM_SUCCESS(hr) ((hr) == S_OK)

#define COM_FAIL(hr) ((hr) != S_OK)

#define if_COM(type, name, func) \
if (type name; COM_SUCCESS(func)) 

#define if_COM_RAW(type, name, func) \
if (type name; (func)) 

#define if_COMR(type, name, func) \
if_COM(type, name, func) if (COMReleaser<type> MACRO_CAT(name, _releaser){name}; 0); else

#define if_COMR_RAW(type, name, func) \
if_COM_RAW(type, name, func) if (COMReleaser<type> MACRO_CAT(name, _releaser){name}; 0); else

bool set_voice5_speaker(ISpVoice* voice, const char* name) {
	if_COMR(ISpObjectTokenCategory*, category, CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, __uuidof(ISpObjectTokenCategory), (LPVOID*)&category)) {
		if (COM_SUCCESS(category->SetId(SPCAT_VOICES, FALSE))) {
			if_COMR(IEnumSpObjectTokens*, token_enum, category->EnumTokens(NULL, NULL, &token_enum)) {
				if_COM(ULONG, count, token_enum->GetCount(&count)) {
					wchar_t* wide_name;
					if (auto name_lookup = voice5_lookup.find(name);
						name_lookup != voice5_lookup.end()
					) {
						wide_name = (wchar_t*)name_lookup->second;
					}
					else {
						size_t length = strlen(name) + 1;
						wide_name = (wchar_t*)alloca(sizeof(wchar_t[length]));
						for (size_t i = 0; i < length; ++i) {
							wide_name[i] = (unsigned char)name[i];
						}
					}
					while (count--) {
						if_COMR(ISpObjectToken*, token, token_enum->Next(1, &token, NULL)) {
							if_COM(WCHAR*, name_string, token->GetStringValue(NULL, &name_string)) {
								if (wcscmp(wide_name, name_string) == 0) {
									voice->SetVoice(token);
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

gnu_noinline void get_voices5() {

	if_COMR(ISpObjectTokenCategory*, category, CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, __uuidof(ISpObjectTokenCategory), (LPVOID*)&category)) {
		if (COM_SUCCESS(category->SetId(SPCAT_VOICES, FALSE))) {
			if_COMR(IEnumSpObjectTokens*, token_enum, category->EnumTokens(NULL, NULL, &token_enum)) {
				if_COM(ULONG, count, token_enum->GetCount(&count)) {
					while (count--) {
						if_COMR(ISpObjectToken*, token, token_enum->Next(1, &token, NULL)) {
							if_COM(WCHAR*, name_string, token->GetStringValue(NULL, &name_string)) {
								voice_names.emplace_back(name_string);
							}
						}
					}
				}
			}
		}
	}
}

gnu_noinline void get_voices4() {
	if_COMR(PITTSENUM, pTTS4_enum, CoCreateInstance(
		CLSID_TTSEnumerator,
		NULL,
		CLSCTX_ALL,
		IID_ITTSEnum,
		(LPVOID*)&pTTS4_enum
	)) {
		for (TTSMODEINFO nTTS_info; COM_SUCCESS(pTTS4_enum->Next(1, &nTTS_info, NULL));) {
			const char* name = nTTS_info.szModeName;
			size_t length = strlen(name) + 1;
			wchar_t wide_name[length];
			for (size_t i = 0; i < length; ++i) {
				wide_name[i] = (unsigned char)name[i];
			}
			voice_names.emplace_back(&wide_name[0]);
		}
	}
}

struct TTSNotifyCrap : public ITTSNotifySink {
	volatile bool voice_done;

	STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppv) {
		*ppv = NULL;

		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITTSNotifySink)) {
			*ppv = (LPVOID)this;
			return S_OK;
		}

		return ResultFromScode(E_NOINTERFACE);
	}
	STDMETHODIMP_(ULONG) AddRef(void) {
		return 1;
	}
	STDMETHODIMP_(ULONG) Release(void) {
		return 1;
	}

	// ITTSNotifySink
	STDMETHOD(AttribChanged) (DWORD dwAttribID) {
		return NOERROR;
	}
	STDMETHOD(AudioStart) (QWORD qTimeStamp) {
		this->voice_done = false;
		return NOERROR;
	}
	STDMETHOD(AudioStop) (QWORD qTimeStamp) {
		this->voice_done = true;
		return NOERROR;
	}
	STDMETHOD(Visual) (QWORD qTimeStamp, CHAR cIPAPhoneme, CHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth) {
		return NOERROR;
	}

};

gnu_noinline dllexport void PlaySAPI5(const char* voice, const char* text) {
	if_COMR(ISpVoice*, tts_voice, CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&tts_voice)) {
		if (set_voice5_speaker(tts_voice, voice)) {
			size_t length = strlen(text) + 1;
			wchar_t wide_text[length];
			for (size_t i = 0; i < length; ++i) {
				wide_text[i] = (unsigned char)text[i];
			}
			tts_voice->Speak(wide_text, 0, NULL);
			tts_voice->WaitUntilDone(INFINITE);
		}
	}
}

gnu_noinline dllexport void PlaySAPI4(const char* voice, const char* text) {
	if_COMR(PITTSFIND, tts_finder, CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFind, (LPVOID*)&tts_finder)) {
		TTSMODEINFO current_voice;
		memset(&current_voice, 0, sizeof(current_voice));
		strcpy(current_voice.szModeName, voice);

		if (COM_SUCCESS(tts_finder->Find(&current_voice, NULL, &current_voice))) {
			if (strcmp(voice, current_voice.szModeName) != 0) {
				if_COMR(PITTSENUM, tts_enum, CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnum, (LPVOID*)&tts_enum)) {
					while (COM_SUCCESS(tts_enum->Next(1, &current_voice, NULL))) {
						if (strcmp(voice, current_voice.szModeName) == 0) {
							break;
						}
					}
				}
			}
		}
		
		if_COMR(PIAUDIOMULTIMEDIADEVICE, audio_out, CoCreateInstance(CLSID_MMAudioDest, NULL, CLSCTX_ALL, IID_IAudioMultiMediaDevice, (LPVOID*)&audio_out)) {
			if_COMR(PITTSCENTRAL, tts_player, tts_finder->Select(current_voice.gModeID, &tts_player, audio_out)) {
				if (PITTSATTRIBUTES tts_attributes; COM_SUCCESS(tts_player->QueryInterface(IID_ITTSAttributes, (LPVOID*)&tts_attributes))) {
					DWORD idgaf;
					TTSNotifyCrap notifier;
					tts_player->Register((LPVOID)&notifier, IID_ITTSNotifySink, &idgaf);

					tts_player->AudioReset();
					tts_player->TextData(CHARSET_TEXT, TTSDATAFLAG_TAGGED, (SDATA){ (PVOID)text , strlen(text) + 1 }, NULL, IID_ITTSBufNotifySink);
					
					while (!notifier.voice_done) {
						parse_windows_messages();
					}
				}
			}
		}
	}
}

struct VoicePlayer : public ITTSNotifySinkW {
	wchar_t* name;
	volatile bool is_speaking;
	bool is_sapi5;
	union {
		ISpVoice* sapi5_voice;
		PITTSCENTRALW sapi4_voice;
	};
	union {
		HANDLE sapi5_is_speaking_handle;
		GUID sapi4_guid;
	};

	~VoicePlayer() {
		//this->sapi5_voice->Release(); // Will work the same for sapi4 because vtable jank
	}

	VoicePlayer(wchar_t* _name, ISpVoice* _sapi5_voice)
		: name(_name), is_speaking(false), is_sapi5(true), sapi5_voice(_sapi5_voice), sapi5_is_speaking_handle(INVALID_HANDLE_VALUE) {}
	VoicePlayer(wchar_t* _name, ISpVoice* _sapi5_voice, HANDLE _sapi5_is_speaking_handle)
		: name(_name), is_speaking(false), is_sapi5(true), sapi5_voice(_sapi5_voice), sapi5_is_speaking_handle(_sapi5_is_speaking_handle) {}
	VoicePlayer(wchar_t* _name, PITTSCENTRALW _sapi4_voice)
		: name(_name), is_speaking(false), is_sapi5(false), sapi4_voice(_sapi4_voice), sapi5_is_speaking_handle(INVALID_HANDLE_VALUE) {}
	VoicePlayer(wchar_t* _name, PITTSCENTRALW _sapi4_voice, GUID _sapi4_guid)
		: name(_name), is_speaking(false), is_sapi5(false), sapi4_voice(_sapi4_voice), sapi4_guid(_sapi4_guid) {}

	void speak4(const wchar_t* text, PITTSENUMW sapi4_enum, PIAUDIOMULTIMEDIADEVICE sapi4_audio_device) {

		// TODO: Figure out why TF these lines need to be rerun every time
		sapi4_enum->Select(this->sapi4_guid, &this->sapi4_voice, sapi4_audio_device);
		DWORD callback_id;
		this->sapi4_voice->Register((LPVOID)this, IID_ITTSNotifySinkW, &callback_id);


		this->sapi4_voice->AudioReset();
		this->sapi4_voice->TextData(CHARSET_TEXT, TTSDATAFLAG_TAGGED, (SDATA) { (PVOID)text, sizeof(wchar_t[wcslen(text) + 1]) }, NULL, IID_ITTSBufNotifySink);
		this->is_speaking = true;
		while (this->is_speaking) {
			parse_windows_messages();
		}
		//this->sapi4_voice->UnRegister(callback_id);
	}

	void speak5(const wchar_t* text) {
		this->sapi5_voice->Speak(text, SPF_PURGEBEFORESPEAK, NULL);
		this->sapi5_voice->WaitUntilDone(INFINITE);
	}

	void speak(const wchar_t* text, PITTSENUMW sapi4_enum, PIAUDIOMULTIMEDIADEVICE sapi4_audio_device) {
		this->is_sapi5 ? this->speak5(text) : this->speak4(text, sapi4_enum, sapi4_audio_device);
	}

	STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppv) {
		*ppv = NULL;

		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITTSNotifySinkW)) {
			*ppv = (LPVOID)this;
			return S_OK;
		}

		return ResultFromScode(E_NOINTERFACE);
	}
	STDMETHODIMP_(ULONG) AddRef(void) {
		return 1;
	}
	STDMETHODIMP_(ULONG) Release(void) {
		return 1;
	}

	// ITTSNotifySink
	STDMETHOD(AttribChanged) (DWORD dwAttribID) {
		return NOERROR;
	}
	STDMETHOD(AudioStart) (QWORD qTimeStamp) {
		this->is_speaking = true;
		return NOERROR;
	}
	STDMETHOD(AudioStop) (QWORD qTimeStamp) {
		this->is_speaking = false;
		return NOERROR;
	}
	STDMETHOD(Visual) (QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth) {
		return NOERROR;
	}
};

struct VoiceManager {
	PIAUDIOMULTIMEDIADEVICE sapi4_audio_device;
	PITTSENUMW sapi4_enum;
	ISpObjectTokenCategory* sapi5_category;
	std::vector<VoicePlayer> voice_players;

	void speak_as(VoicePlayer& voice, const wchar_t* text) {
		voice.speak(text, this->sapi4_enum, this->sapi4_audio_device);
	}

	void speak_as(size_t index, const wchar_t* text) {
		this->voice_players[index].speak(text, this->sapi4_enum, this->sapi4_audio_device);
	}

	gnu_noinline void initialize() {
		this->sapi4_audio_device = NULL;
		this->sapi4_enum = NULL;
		this->sapi5_category = NULL;
		this->voice_players.clear();

		// TODO: Figure out why this loop is cursed and the one in PlaySAPI4 isn't
		if (COM_SUCCESS(CoCreateInstance(CLSID_MMAudioDest, NULL, CLSCTX_ALL, IID_IAudioMultiMediaDevice, (LPVOID*)&this->sapi4_audio_device))) {
			if (COM_SUCCESS(CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnumW, (LPVOID*)&this->sapi4_enum))) {
				for (TTSMODEINFOW voice_data; COM_SUCCESS(this->sapi4_enum->Next(1, &voice_data, NULL));) {
					if_COM(PITTSCENTRALW, sapi4_player, this->sapi4_enum->Select(voice_data.gModeID, &sapi4_player, this->sapi4_audio_device)) {
						this->voice_players.emplace_back(wcsdup(voice_data.szModeName), nullptr, voice_data.gModeID);
					}
					else {
						printf("Cursed Fail: %ls\n", voice_data.szModeName);
					}
				}
			}
		}
		if (COM_SUCCESS(CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (LPVOID*)&this->sapi5_category))) {
			if (COM_SUCCESS(this->sapi5_category->SetId(SPCAT_VOICES, FALSE))) {
				if_COMR(IEnumSpObjectTokens*, sapi5_enum, this->sapi5_category->EnumTokens(NULL, NULL, &sapi5_enum)) {
					if_COM(ULONG, count, sapi5_enum->GetCount(&count)) {
						while (count--) {
							if_COM(ISpObjectToken*, sapi5_token, sapi5_enum->Next(1, &sapi5_token, NULL)) {
								if_COM(WCHAR*, name_string, sapi5_token->GetStringValue(NULL, &name_string)) {
									if_COM(ISpVoice*, sapi5_player, CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (LPVOID*)&sapi5_player)) {
										sapi5_player->SetVoice(sapi5_token);
										this->voice_players.emplace_back(wcsdup(name_string), sapi5_player, sapi5_player->SpeakCompleteEvent());
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void speak_as_all(const wchar_t* text) {
		for (auto& voice : this->voice_players) {
			printf("%ls\n", voice.name);
			this->speak_as(voice, text);
		}
	}

	~VoiceManager() {
		if (auto temp = this->sapi5_category) temp->Release();
		if (auto temp = this->sapi4_audio_device) temp->Release();
	}
};

gnu_noinline void tts_test() {

	voice_names.clear();

	if_COMR(ISpVoice*, pVoice, CoCreateInstance(
		CLSID_SpVoice,
		NULL,
		CLSCTX_ALL,
		IID_ISpVoice,
		(LPVOID*)&pVoice
	)) {
		get_voices5();
	}

	get_voices4();

	print_voice_list();

	PlaySAPI4("Sam", "yeetus deleetus");
}

gnu_noinline void tts_manager_test() {
	VoiceManager voice_manager;
	voice_manager.initialize();
	voice_manager.speak_as_all(L"yeetus deleetus");
}

int main(int argc, char* argv[]) {
	if (COM_SUCCESS(CoInitialize(NULL))) {
		//tts_test();
		tts_manager_test();
		CoUninitialize();
	}
	return 0;
}