#pragma once
#pragma clang diagnostic ignored "-Wdll-attribute-on-redeclaration"

// Nasty hack header to force clang to properly emit indirect calls
// by spamming __declspec(dllimport) on every function.

#if !__INTELLISENSE__
extern "C" {
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2Normalize
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2Hermite
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV1, CONST D3DXVECTOR2 *pT1,
      CONST D3DXVECTOR2 *pV2, CONST D3DXVECTOR2 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2CatmullRom
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV0, CONST D3DXVECTOR2 *pV1,
      CONST D3DXVECTOR2 *pV2, CONST D3DXVECTOR2 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2BaryCentric
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV1, CONST D3DXVECTOR2 *pV2,
      CONST D3DXVECTOR2 *pV3, FLOAT f, FLOAT g);

// Transform (x, y, 0, 1) by matrix.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec2Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, 0, 1) by matrix, project result back into w=1.
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2TransformCoord
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, 0, 0) by matrix.
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2TransformNormal
    ( D3DXVECTOR2 *pOut, CONST D3DXVECTOR2 *pV, CONST D3DXMATRIX *pM );
     
// Transform Array (x, y, 0, 1) by matrix.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec2TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n);

// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2TransformCoordArray
    ( D3DXVECTOR2 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform Array (x, y, 0, 0) by matrix.
__declspec(dllimport)
D3DXVECTOR2* WINAPI D3DXVec2TransformNormalArray
    ( D3DXVECTOR2 *pOut, UINT OutStride, CONST D3DXVECTOR2 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3Normalize
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3Hermite
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pT1,
      CONST D3DXVECTOR3 *pV2, CONST D3DXVECTOR3 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3CatmullRom
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV0, CONST D3DXVECTOR3 *pV1,
      CONST D3DXVECTOR3 *pV2, CONST D3DXVECTOR3 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3BaryCentric
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2,
      CONST D3DXVECTOR3 *pV3, FLOAT f, FLOAT g);

// Transform (x, y, z, 1) by matrix.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec3Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, z, 1) by matrix, project result back into w=1.
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3TransformCoord
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3TransformNormal
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );
    
    
// Transform Array (x, y, z, 1) by matrix. 
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec3TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3TransformCoordArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3TransformNormalArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

// Project vector from object space into screen space
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3Project
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld);

// Project vector from screen space into object space
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3Unproject
    ( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld);
      
// Project vector Array from object space into screen space
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3ProjectArray
    ( D3DXVECTOR3 *pOut, UINT OutStride,CONST D3DXVECTOR3 *pV, UINT VStride,CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld, UINT n);

// Project vector Array from screen space into object space
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXVec3UnprojectArray
    ( D3DXVECTOR3 *pOut, UINT OutStride, CONST D3DXVECTOR3 *pV, UINT VStride, CONST D3DVIEWPORT9 *pViewport,
      CONST D3DXMATRIX *pProjection, CONST D3DXMATRIX *pView, CONST D3DXMATRIX *pWorld, UINT n);

// Cross-product in 4 dimensions.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4Cross
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2,
      CONST D3DXVECTOR4 *pV3);

__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4Normalize
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV );

// Hermite interpolation between position V1, tangent T1 (when s == 0)
// and position V2, tangent T2 (when s == 1).
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4Hermite
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pT1,
      CONST D3DXVECTOR4 *pV2, CONST D3DXVECTOR4 *pT2, FLOAT s );

// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4CatmullRom
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV0, CONST D3DXVECTOR4 *pV1,
      CONST D3DXVECTOR4 *pV2, CONST D3DXVECTOR4 *pV3, FLOAT s );

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4BaryCentric
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV1, CONST D3DXVECTOR4 *pV2,
      CONST D3DXVECTOR4 *pV3, FLOAT f, FLOAT g);

// Transform vector by matrix.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4Transform
    ( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV, CONST D3DXMATRIX *pM );
    
// Transform vector array by matrix.
__declspec(dllimport)
D3DXVECTOR4* WINAPI D3DXVec4TransformArray
    ( D3DXVECTOR4 *pOut, UINT OutStride, CONST D3DXVECTOR4 *pV, UINT VStride, CONST D3DXMATRIX *pM, UINT n );

__declspec(dllimport)
FLOAT WINAPI D3DXMatrixDeterminant
    ( CONST D3DXMATRIX *pM );

__declspec(dllimport)
HRESULT WINAPI D3DXMatrixDecompose
    ( D3DXVECTOR3 *pOutScale, D3DXQUATERNION *pOutRotation, 
	  D3DXVECTOR3 *pOutTranslation, CONST D3DXMATRIX *pM );

__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixTranspose
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM );

// Matrix multiplication.  The result represents the transformation M2
// followed by the transformation M1.  (Out = M1 * M2)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixMultiply
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2 );

// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixMultiplyTranspose
    ( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2 );

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixInverse
    ( D3DXMATRIX *pOut, FLOAT *pDeterminant, CONST D3DXMATRIX *pM );

// Build a matrix which scales by (sx, sy, sz)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixScaling
    ( D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz );

// Build a matrix which translates by (x, y, z)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixTranslation
    ( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z );

// Build a matrix which rotates around the X axis
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationX
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around the Y axis
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationY
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around the Z axis
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationZ
    ( D3DXMATRIX *pOut, FLOAT Angle );

// Build a matrix which rotates around an arbitrary axis
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationAxis
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle );

// Build a matrix from a quaternion
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationQuaternion
    ( D3DXMATRIX *pOut, CONST D3DXQUATERNION *pQ);

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixRotationYawPitchRoll
    ( D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll );

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixTransformation
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pScalingCenter,
      CONST D3DXQUATERNION *pScalingRotation, CONST D3DXVECTOR3 *pScaling,
      CONST D3DXVECTOR3 *pRotationCenter, CONST D3DXQUATERNION *pRotation,
      CONST D3DXVECTOR3 *pTranslation);

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixTransformation2D
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR2* pScalingCenter, 
      FLOAT ScalingRotation, CONST D3DXVECTOR2* pScaling, 
      CONST D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
      CONST D3DXVECTOR2* pTranslation);

// Build affine transformation matrix.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation
    ( D3DXMATRIX *pOut, FLOAT Scaling, CONST D3DXVECTOR3 *pRotationCenter,
      CONST D3DXQUATERNION *pRotation, CONST D3DXVECTOR3 *pTranslation);

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation2D
    ( D3DXMATRIX *pOut, FLOAT Scaling, CONST D3DXVECTOR2* pRotationCenter, 
      FLOAT Rotation, CONST D3DXVECTOR2* pTranslation);

// Build a lookat matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixLookAtRH
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
      CONST D3DXVECTOR3 *pUp );

// Build a lookat matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixLookAtLH
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
      CONST D3DXVECTOR3 *pUp );

// Build a perspective projection matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveRH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveLH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovRH
    ( D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH
    ( D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterRH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a perspective projection matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterLH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixOrthoRH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixOrthoLH
    ( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (right-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterRH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (left-handed)
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterLH
    ( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixShadow
    ( D3DXMATRIX *pOut, CONST D3DXVECTOR4 *pLight,
      CONST D3DXPLANE *pPlane );

// Build a matrix which reflects the coordinate system about a plane
__declspec(dllimport)
D3DXMATRIX* WINAPI D3DXMatrixReflect
    ( D3DXMATRIX *pOut, CONST D3DXPLANE *pPlane );

// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
__declspec(dllimport)
void WINAPI D3DXQuaternionToAxisAngle
    ( CONST D3DXQUATERNION *pQ, D3DXVECTOR3 *pAxis, FLOAT *pAngle );

// Build a quaternion from a rotation matrix.
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionRotationMatrix
    ( D3DXQUATERNION *pOut, CONST D3DXMATRIX *pM);

// Rotation about arbitrary axis.
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionRotationAxis
    ( D3DXQUATERNION *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle );

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionRotationYawPitchRoll
    ( D3DXQUATERNION *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll );

// Quaternion multiplication.  The result represents the rotation Q2
// followed by the rotation Q1.  (Out = Q2 * Q1)
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionMultiply
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2 );

__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionNormalize
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Conjugate and re-norm
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionInverse
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Expects unit quaternions.
// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionLn
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );

// Expects pure quaternions. (w == 0)  w is ignored in calculation.
// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionExp
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ );
      
// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
// Expects unit quaternions.
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionSlerp
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2, FLOAT t );

// Spherical quadrangle interpolation.
// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionSquad
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pA, CONST D3DXQUATERNION *pB,
      CONST D3DXQUATERNION *pC, FLOAT t );

// Setup control points for spherical quadrangle interpolation
// from Q1 to Q2.  The control points are chosen in such a way 
// to ensure the continuity of tangents with adjacent segments.
__declspec(dllimport)
void WINAPI D3DXQuaternionSquadSetup
    ( D3DXQUATERNION *pAOut, D3DXQUATERNION *pBOut, D3DXQUATERNION *pCOut,
      CONST D3DXQUATERNION *pQ0, CONST D3DXQUATERNION *pQ1, 
      CONST D3DXQUATERNION *pQ2, CONST D3DXQUATERNION *pQ3 );

// Barycentric interpolation.
// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
__declspec(dllimport)
D3DXQUATERNION* WINAPI D3DXQuaternionBaryCentric
    ( D3DXQUATERNION *pOut, CONST D3DXQUATERNION *pQ1,
      CONST D3DXQUATERNION *pQ2, CONST D3DXQUATERNION *pQ3,
      FLOAT f, FLOAT g );

// Normalize plane (so that |a,b,c| == 1)
__declspec(dllimport)
D3DXPLANE* WINAPI D3DXPlaneNormalize
    ( D3DXPLANE *pOut, CONST D3DXPLANE *pP);

// Find the intersection between a plane and a line.  If the line is
// parallel to the plane, NULL is returned.
__declspec(dllimport)
D3DXVECTOR3* WINAPI D3DXPlaneIntersectLine
    ( D3DXVECTOR3 *pOut, CONST D3DXPLANE *pP, CONST D3DXVECTOR3 *pV1,
      CONST D3DXVECTOR3 *pV2);

// Construct a plane from a point and a normal
__declspec(dllimport)
D3DXPLANE* WINAPI D3DXPlaneFromPointNormal
    ( D3DXPLANE *pOut, CONST D3DXVECTOR3 *pPoint, CONST D3DXVECTOR3 *pNormal);

// Construct a plane from 3 points
__declspec(dllimport)
D3DXPLANE* WINAPI D3DXPlaneFromPoints
    ( D3DXPLANE *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2,
      CONST D3DXVECTOR3 *pV3);

// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
__declspec(dllimport)
D3DXPLANE* WINAPI D3DXPlaneTransform
    ( D3DXPLANE *pOut, CONST D3DXPLANE *pP, CONST D3DXMATRIX *pM );
    
// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
__declspec(dllimport)
D3DXPLANE* WINAPI D3DXPlaneTransformArray
    ( D3DXPLANE *pOut, UINT OutStride, CONST D3DXPLANE *pP, UINT PStride, CONST D3DXMATRIX *pM, UINT n );

// Interpolate r,g,b between desaturated color and color.
// DesaturatedColor + s(Color - DesaturatedColor)
__declspec(dllimport)
D3DXCOLOR* WINAPI D3DXColorAdjustSaturation
    (D3DXCOLOR *pOut, CONST D3DXCOLOR *pC, FLOAT s);

// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
__declspec(dllimport)
D3DXCOLOR* WINAPI D3DXColorAdjustContrast
    (D3DXCOLOR *pOut, CONST D3DXCOLOR *pC, FLOAT c);

// Calculate Fresnel term given the cosine of theta (likely obtained by
// taking the dot of two normals), and the refraction index of the material.
__declspec(dllimport)
FLOAT WINAPI D3DXFresnelTerm
    (FLOAT CosTheta, FLOAT RefractionIndex);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateMatrixStack( 
        DWORD               Flags, 
        LPD3DXMATRIXSTACK*  ppStack);

//============================================================================
//
//  D3DXSHEvalDirection:
//  --------------------
//  Evaluates the Spherical Harmonic basis functions
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned.
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pDir
//      Direction to evaluate in - assumed to be normalized
//
//============================================================================
__declspec(dllimport)
FLOAT* WINAPI D3DXSHEvalDirection
    (  FLOAT *pOut, UINT Order, CONST D3DXVECTOR3 *pDir );
    
//============================================================================
//
//  D3DXSHRotate:
//  --------------------
//  Rotates SH vector by a rotation matrix
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned (should not alias with pIn.)
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pMatrix
//      Matrix used for rotation - rotation sub matrix should be orthogonal
//      and have a unit determinant.
//   pIn
//      Input SH coeffs (rotated), incorect results if this is also output.
//
//============================================================================
__declspec(dllimport)
FLOAT* WINAPI D3DXSHRotate
    ( FLOAT *pOut, UINT Order, CONST D3DXMATRIX *pMatrix, CONST FLOAT *pIn );
    
//============================================================================
//
//  D3DXSHRotateZ:
//  --------------------
//  Rotates the SH vector in the Z axis by an angle
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned (should not alias with pIn.)
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   Angle
//      Angle in radians to rotate around the Z axis.
//   pIn
//      Input SH coeffs (rotated), incorect results if this is also output.
//
//============================================================================
__declspec(dllimport)
FLOAT* WINAPI D3DXSHRotateZ
    ( FLOAT *pOut, UINT Order, FLOAT Angle, CONST FLOAT *pIn );
    
//============================================================================
//
//  D3DXSHAdd:
//  --------------------
//  Adds two SH vectors, pOut[i] = pA[i] + pB[i];
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned.
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pA
//      Input SH coeffs.
//   pB
//      Input SH coeffs (second vector.)
//
//============================================================================
__declspec(dllimport)
FLOAT* WINAPI D3DXSHAdd
    ( FLOAT *pOut, UINT Order, CONST FLOAT *pA, CONST FLOAT *pB );

//============================================================================
//
//  D3DXSHScale:
//  --------------------
//  Adds two SH vectors, pOut[i] = pA[i]*Scale;
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned.
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pIn
//      Input SH coeffs.
//   Scale
//      Scale factor.
//
//============================================================================
__declspec(dllimport)
FLOAT* WINAPI D3DXSHScale
    ( FLOAT *pOut, UINT Order, CONST FLOAT *pIn, CONST FLOAT Scale );
    
//============================================================================
//
//  D3DXSHDot:
//  --------------------
//  Computes the dot product of two SH vectors
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pA
//      Input SH coeffs.
//   pB
//      Second set of input SH coeffs.
//
//============================================================================
__declspec(dllimport)
FLOAT WINAPI D3DXSHDot
    ( UINT Order, CONST FLOAT *pA, CONST FLOAT *pB );

//============================================================================
//
//  D3DXSHMultiply[O]:
//  --------------------
//  Computes the product of two functions represented using SH (f and g), where:
//  pOut[i] = int(y_i(s) * f(s) * g(s)), where y_i(s) is the ith SH basis
//  function, f(s) and g(s) are SH functions (sum_i(y_i(s)*c_i)).  The order O
//  determines the lengths of the arrays, where there should always be O^2 
//  coefficients.  In general the product of two SH functions of order O generates
//  and SH function of order 2*O - 1, but we truncate the result.  This means
//  that the product commutes (f*g == g*f) but doesn't associate 
//  (f*(g*h) != (f*g)*h.
//
//  Parameters:
//   pOut
//      Output SH coefficients - basis function Ylm is stored at l*l + m+l
//      This is the pointer that is returned.
//   pF
//      Input SH coeffs for first function.
//   pG
//      Second set of input SH coeffs.
//
//============================================================================

__declspec(dllimport)
FLOAT* WINAPI D3DXSHMultiply2( FLOAT *pOut, CONST FLOAT *pF, CONST FLOAT *pG);
__declspec(dllimport)
FLOAT* WINAPI D3DXSHMultiply3( FLOAT *pOut, CONST FLOAT *pF, CONST FLOAT *pG);
__declspec(dllimport)
FLOAT* WINAPI D3DXSHMultiply4( FLOAT *pOut, CONST FLOAT *pF, CONST FLOAT *pG);
__declspec(dllimport)
FLOAT* WINAPI D3DXSHMultiply5( FLOAT *pOut, CONST FLOAT *pF, CONST FLOAT *pG);
__declspec(dllimport)
FLOAT* WINAPI D3DXSHMultiply6( FLOAT *pOut, CONST FLOAT *pF, CONST FLOAT *pG);

//============================================================================
//
//  D3DXSHEvalDirectionalLight:
//  --------------------
//  Evaluates a directional light and returns spectral SH data.  The output 
//  vector is computed so that if the intensity of R/G/B is unit the resulting
//  exit radiance of a point directly under the light on a diffuse object with
//  an albedo of 1 would be 1.0.  This will compute 3 spectral samples, pROut
//  has to be specified, while pGout and pBout are optional.
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pDir
//      Direction light is coming from (assumed to be normalized.)
//   RIntensity
//      Red intensity of light.
//   GIntensity
//      Green intensity of light.
//   BIntensity
//      Blue intensity of light.
//   pROut
//      Output SH vector for Red.
//   pGOut
//      Output SH vector for Green (optional.)
//   pBOut
//      Output SH vector for Blue (optional.)        
//
//============================================================================
__declspec(dllimport)
HRESULT WINAPI D3DXSHEvalDirectionalLight
    ( UINT Order, CONST D3DXVECTOR3 *pDir, 
      FLOAT RIntensity, FLOAT GIntensity, FLOAT BIntensity,
      FLOAT *pROut, FLOAT *pGOut, FLOAT *pBOut );

//============================================================================
//
//  D3DXSHEvalSphericalLight:
//  --------------------
//  Evaluates a spherical light and returns spectral SH data.  There is no 
//  normalization of the intensity of the light like there is for directional
//  lights, care has to be taken when specifiying the intensities.  This will 
//  compute 3 spectral samples, pROut has to be specified, while pGout and 
//  pBout are optional.
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pPos
//      Position of light - reciever is assumed to be at the origin.
//   Radius
//      Radius of the spherical light source.
//   RIntensity
//      Red intensity of light.
//   GIntensity
//      Green intensity of light.
//   BIntensity
//      Blue intensity of light.
//   pROut
//      Output SH vector for Red.
//   pGOut
//      Output SH vector for Green (optional.)
//   pBOut
//      Output SH vector for Blue (optional.)        
//
//============================================================================
__declspec(dllimport)
HRESULT WINAPI D3DXSHEvalSphericalLight
    ( UINT Order, CONST D3DXVECTOR3 *pPos, FLOAT Radius,
      FLOAT RIntensity, FLOAT GIntensity, FLOAT BIntensity,
      FLOAT *pROut, FLOAT *pGOut, FLOAT *pBOut );

//============================================================================
//
//  D3DXSHEvalConeLight:
//  --------------------
//  Evaluates a light that is a cone of constant intensity and returns spectral
//  SH data.  The output vector is computed so that if the intensity of R/G/B is
//  unit the resulting exit radiance of a point directly under the light oriented
//  in the cone direction on a diffuse object with an albedo of 1 would be 1.0.
//  This will compute 3 spectral samples, pROut has to be specified, while pGout
//  and pBout are optional.
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pDir
//      Direction light is coming from (assumed to be normalized.)
//   Radius
//      Radius of cone in radians.
//   RIntensity
//      Red intensity of light.
//   GIntensity
//      Green intensity of light.
//   BIntensity
//      Blue intensity of light.
//   pROut
//      Output SH vector for Red.
//   pGOut
//      Output SH vector for Green (optional.)
//   pBOut
//      Output SH vector for Blue (optional.)        
//
//============================================================================
__declspec(dllimport)
HRESULT WINAPI D3DXSHEvalConeLight
    ( UINT Order, CONST D3DXVECTOR3 *pDir, FLOAT Radius,
      FLOAT RIntensity, FLOAT GIntensity, FLOAT BIntensity,
      FLOAT *pROut, FLOAT *pGOut, FLOAT *pBOut );
      
//============================================================================
//
//  D3DXSHEvalHemisphereLight:
//  --------------------
//  Evaluates a light that is a linear interpolant between two colors over the
//  sphere.  The interpolant is linear along the axis of the two points, not
//  over the surface of the sphere (ie: if the axis was (0,0,1) it is linear in
//  Z, not in the azimuthal angle.)  The resulting spherical lighting function
//  is normalized so that a point on a perfectly diffuse surface with no
//  shadowing and a normal pointed in the direction pDir would result in exit
//  radiance with a value of 1 if the top color was white and the bottom color
//  was black.  This is a very simple model where Top represents the intensity 
//  of the "sky" and Bottom represents the intensity of the "ground".
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pDir
//      Axis of the hemisphere.
//   Top
//      Color of the upper hemisphere.
//   Bottom
//      Color of the lower hemisphere.
//   pROut
//      Output SH vector for Red.
//   pGOut
//      Output SH vector for Green
//   pBOut
//      Output SH vector for Blue        
//
//============================================================================
__declspec(dllimport)
HRESULT WINAPI D3DXSHEvalHemisphereLight
    ( UINT Order, CONST D3DXVECTOR3 *pDir, D3DXCOLOR Top, D3DXCOLOR Bottom,
      FLOAT *pROut, FLOAT *pGOut, FLOAT *pBOut );

//============================================================================
//
//  D3DXSHProjectCubeMap:
//  --------------------
//  Projects a function represented on a cube map into spherical harmonics.
//
//  Parameters:
//   Order
//      Order of the SH evaluation, generates Order^2 coefs, degree is Order-1
//   pCubeMap
//      CubeMap that is going to be projected into spherical harmonics
//   pROut
//      Output SH vector for Red.
//   pGOut
//      Output SH vector for Green
//   pBOut
//      Output SH vector for Blue        
//
//============================================================================
__declspec(dllimport)
HRESULT WINAPI D3DXSHProjectCubeMap
    ( UINT uOrder, LPDIRECT3DCUBETEXTURE9 pCubeMap,
      FLOAT *pROut, FLOAT *pGOut, FLOAT *pBOut );

__declspec(dllimport)
BOOL WINAPI
    D3DXCheckVersion(UINT D3DSdkVersion, UINT D3DXSdkVersion);

__declspec(dllimport)
BOOL WINAPI
    D3DXDebugMute(BOOL Mute);  

__declspec(dllimport)
UINT WINAPI
    D3DXGetDriverLevel(LPDIRECT3DDEVICE9 pDevice);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateSprite( 
        LPDIRECT3DDEVICE9   pDevice, 
        LPD3DXSPRITE*       ppSprite);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateFontA(
        LPDIRECT3DDEVICE9       pDevice,  
        INT                     Height,
        UINT                    Width,
        UINT                    Weight,
        UINT                    MipLevels,
        BOOL                    Italic,
        DWORD                   CharSet,
        DWORD                   OutputPrecision,
        DWORD                   Quality,
        DWORD                   PitchAndFamily,
        LPCSTR                  pFaceName,
        LPD3DXFONT*             ppFont);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateFontW(
        LPDIRECT3DDEVICE9       pDevice,  
        INT                     Height,
        UINT                    Width,
        UINT                    Weight,
        UINT                    MipLevels,
        BOOL                    Italic,
        DWORD                   CharSet,
        DWORD                   OutputPrecision,
        DWORD                   Quality,
        DWORD                   PitchAndFamily,
        LPCWSTR                 pFaceName,
        LPD3DXFONT*             ppFont);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateFontIndirectA( 
        LPDIRECT3DDEVICE9       pDevice, 
        CONST D3DXFONT_DESCA*   pDesc, 
        LPD3DXFONT*             ppFont);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateFontIndirectW( 
        LPDIRECT3DDEVICE9       pDevice, 
        CONST D3DXFONT_DESCW*   pDesc, 
        LPD3DXFONT*             ppFont);

__declspec(dllimport)
STDAPI D3DXFileCreate(ID3DXFile** lplpDirectXFile);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateMesh(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        CONST D3DVERTEXELEMENT9 *pDeclaration, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXMESH* ppMesh);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateMeshFVF(
        DWORD NumFaces, 
        DWORD NumVertices, 
        DWORD Options, 
        DWORD FVF, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXMESH* ppMesh);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateSPMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        LPD3DXSPMESH* ppSMesh);


__declspec(dllimport)
HRESULT WINAPI
    D3DXCleanMesh(
    D3DXCLEANTYPE CleanType,
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacencyIn,
    LPD3DXMESH* ppMeshOut,
    DWORD* pAdjacencyOut,
    LPD3DXBUFFER* ppErrorsAndWarnings);


__declspec(dllimport)
HRESULT WINAPI
    D3DXValidMesh(
    LPD3DXMESH pMeshIn,
    CONST DWORD* pAdjacency,
    LPD3DXBUFFER* ppErrorsAndWarnings);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXGeneratePMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXPMESH* ppPMesh);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXSimplifyMesh(
        LPD3DXMESH pMesh, 
        CONST DWORD* pAdjacency, 
        CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
        CONST FLOAT *pVertexWeights,
        DWORD MinValue, 
        DWORD Options, 
        LPD3DXMESH* ppMesh);


__declspec(dllimport)
HRESULT WINAPI 
    D3DXComputeBoundingSphere(
        CONST D3DXVECTOR3 *pFirstPosition,  // pointer to first position
        DWORD NumVertices, 
        DWORD dwStride,                     // count in bytes to subsequent position vectors
        D3DXVECTOR3 *pCenter, 
        FLOAT *pRadius);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXComputeBoundingBox(
        CONST D3DXVECTOR3 *pFirstPosition,  // pointer to first position
        DWORD NumVertices, 
        DWORD dwStride,                     // count in bytes to subsequent position vectors
        D3DXVECTOR3 *pMin, 
        D3DXVECTOR3 *pMax);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXComputeNormals(
        LPD3DXBASEMESH pMesh,
        CONST DWORD *pAdjacency);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateBuffer(
        DWORD NumBytes, 
        LPD3DXBUFFER *ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadMeshFromXA(
        LPCSTR pFilename, 
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadMeshFromXW(
        LPCWSTR pFilename, 
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXLoadMeshFromXInMemory(
        LPCVOID Memory,
        DWORD SizeOfMemory,
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXLoadMeshFromXResource(
        HMODULE Module,
        LPCSTR Name,
        LPCSTR Type,
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXSaveMeshToXA(
        LPCSTR pFilename,
        LPD3DXMESH pMesh,
        CONST DWORD* pAdjacency,
        CONST D3DXMATERIAL* pMaterials,
        CONST D3DXEFFECTINSTANCE* pEffectInstances, 
        DWORD NumMaterials,
        DWORD Format
        );

__declspec(dllimport)
HRESULT WINAPI 
    D3DXSaveMeshToXW(
        LPCWSTR pFilename,
        LPD3DXMESH pMesh,
        CONST DWORD* pAdjacency,
        CONST D3DXMATERIAL* pMaterials,
        CONST D3DXEFFECTINSTANCE* pEffectInstances, 
        DWORD NumMaterials,
        DWORD Format
        );

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreatePMeshFromStream(
        IStream *pStream, 
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD* pNumMaterials,
        LPD3DXPMESH *ppPMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateSkinInfo(
        DWORD NumVertices,
        CONST D3DVERTEXELEMENT9 *pDeclaration, 
        DWORD NumBones,
        LPD3DXSKININFO* ppSkinInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateSkinInfoFVF(
        DWORD NumVertices,
        DWORD FVF,
        DWORD NumBones,
        LPD3DXSKININFO* ppSkinInfo);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXLoadMeshFromXof(
        LPD3DXFILEDATA pxofMesh, 
        DWORD Options, 
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXBUFFER *ppAdjacency,
        LPD3DXBUFFER *ppMaterials, 
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pNumMaterials,
        LPD3DXMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSkinMeshFromXof(
        LPD3DXFILEDATA pxofMesh, 
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice,
        LPD3DXBUFFER* ppAdjacency,
        LPD3DXBUFFER* ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        DWORD *pMatOut,
        LPD3DXSKININFO* ppSkinInfo,
        LPD3DXMESH* ppMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateSkinInfoFromBlendedMesh(
        LPD3DXBASEMESH pMesh,
        DWORD NumBones,
        CONST D3DXBONECOMBINATION *pBoneCombinationTable,
        LPD3DXSKININFO* ppSkinInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXTessellateNPatches(
        LPD3DXMESH pMeshIn,             
        CONST DWORD* pAdjacencyIn,             
        FLOAT NumSegs,                    
        BOOL  QuadraticInterpNormals,     // if false use linear intrep for normals, if true use quadratic
        LPD3DXMESH *ppMeshOut,
        LPD3DXBUFFER *ppAdjacencyOut);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGenerateOutputDecl(
        D3DVERTEXELEMENT9 *pOutput,
        CONST D3DVERTEXELEMENT9 *pInput);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadPatchMeshFromXof(
        LPD3DXFILEDATA pXofObjMesh,
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice,
        LPD3DXBUFFER *ppMaterials,
        LPD3DXBUFFER *ppEffectInstances, 
        PDWORD pNumMaterials,
        LPD3DXPATCHMESH *ppMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXRectPatchSize(
        CONST FLOAT *pfNumSegs, //segments for each edge (4)
        DWORD *pdwTriangles,    //output number of triangles
        DWORD *pdwVertices);    //output number of vertices

__declspec(dllimport)
HRESULT WINAPI
    D3DXTriPatchSize(
        CONST FLOAT *pfNumSegs, //segments for each edge (3)    
        DWORD *pdwTriangles,    //output number of triangles
        DWORD *pdwVertices);    //output number of vertices

__declspec(dllimport)
HRESULT WINAPI
    D3DXTessellateRectPatch(
        LPDIRECT3DVERTEXBUFFER9 pVB,
        CONST FLOAT *pNumSegs,
        CONST D3DVERTEXELEMENT9 *pdwInDecl,
        CONST D3DRECTPATCH_INFO *pRectPatchInfo,
        LPD3DXMESH pMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXTessellateTriPatch(
      LPDIRECT3DVERTEXBUFFER9 pVB,
      CONST FLOAT *pNumSegs,
      CONST D3DVERTEXELEMENT9 *pInDecl,
      CONST D3DTRIPATCH_INFO *pTriPatchInfo,
      LPD3DXMESH pMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateNPatchMesh(
        LPD3DXMESH pMeshSysMem,
        LPD3DXPATCHMESH *pPatchMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreatePatchMesh(
        CONST D3DXPATCHINFO *pInfo,     //patch type
        DWORD dwNumPatches,             //number of patches
        DWORD dwNumVertices,            //number of control vertices
        DWORD dwOptions,                //options 
        CONST D3DVERTEXELEMENT9 *pDecl, //format of control vertices
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXPATCHMESH *pPatchMesh);

__declspec(dllimport)
HRESULT WINAPI
    D3DXValidPatchMesh(LPD3DXPATCHMESH pMesh,
                        DWORD *dwcDegenerateVertices,
                        DWORD *dwcDegeneratePatches,
                        LPD3DXBUFFER *ppErrorsAndWarnings);

__declspec(dllimport)
UINT WINAPI
    D3DXGetFVFVertexSize(DWORD FVF);

__declspec(dllimport)
UINT WINAPI 
    D3DXGetDeclVertexSize(CONST D3DVERTEXELEMENT9 *pDecl,DWORD Stream);

__declspec(dllimport)
UINT WINAPI 
    D3DXGetDeclLength(CONST D3DVERTEXELEMENT9 *pDecl);

__declspec(dllimport)
HRESULT WINAPI
    D3DXDeclaratorFromFVF(
        DWORD FVF,
        D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE]);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFVFFromDeclarator(
        CONST D3DVERTEXELEMENT9 *pDeclarator,
        DWORD *pFVF);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXWeldVertices(
        LPD3DXMESH pMesh,         
        DWORD Flags,
        CONST D3DXWELDEPSILONS *pEpsilons,                 
        CONST DWORD *pAdjacencyIn, 
        DWORD *pAdjacencyOut,
        DWORD *pFaceRemap, 
        LPD3DXBUFFER *ppVertexRemap);

__declspec(dllimport)
HRESULT WINAPI
    D3DXIntersect(
        LPD3DXBASEMESH pMesh,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array

__declspec(dllimport)
HRESULT WINAPI
    D3DXIntersectSubset(
        LPD3DXBASEMESH pMesh,
        DWORD AttribId,
        CONST D3DXVECTOR3 *pRayPos,
        CONST D3DXVECTOR3 *pRayDir, 
        BOOL    *pHit,              // True if any faces were intersected
        DWORD   *pFaceIndex,        // index of closest face intersected
        FLOAT   *pU,                // Barycentric Hit Coordinates    
        FLOAT   *pV,                // Barycentric Hit Coordinates
        FLOAT   *pDist,             // Ray-Intersection Parameter Distance
        LPD3DXBUFFER *ppAllHits,    // Array of D3DXINTERSECTINFOs for all hits (not just closest) 
        DWORD   *pCountOfHits);     // Number of entries in AllHits array

__declspec(dllimport)
HRESULT WINAPI D3DXSplitMesh
    (
    LPD3DXMESH pMeshIn,         
    CONST DWORD *pAdjacencyIn, 
    CONST DWORD MaxSize,
    CONST DWORD Options,
    DWORD *pMeshesOut,
    LPD3DXBUFFER *ppMeshArrayOut,
    LPD3DXBUFFER *ppAdjacencyArrayOut,
    LPD3DXBUFFER *ppFaceRemapArrayOut,
    LPD3DXBUFFER *ppVertRemapArrayOut
    );

__declspec(dllimport)
BOOL WINAPI D3DXIntersectTri 
(
    CONST D3DXVECTOR3 *p0,           // Triangle vertex 0 position
    CONST D3DXVECTOR3 *p1,           // Triangle vertex 1 position
    CONST D3DXVECTOR3 *p2,           // Triangle vertex 2 position
    CONST D3DXVECTOR3 *pRayPos,      // Ray origin
    CONST D3DXVECTOR3 *pRayDir,      // Ray direction
    FLOAT *pU,                       // Barycentric Hit Coordinates
    FLOAT *pV,                       // Barycentric Hit Coordinates
    FLOAT *pDist);                   // Ray-Intersection Parameter Distance

__declspec(dllimport)
BOOL WINAPI
    D3DXSphereBoundProbe(
        CONST D3DXVECTOR3 *pCenter,
        FLOAT Radius,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);

__declspec(dllimport)
BOOL WINAPI 
    D3DXBoxBoundProbe(
        CONST D3DXVECTOR3 *pMin, 
        CONST D3DXVECTOR3 *pMax,
        CONST D3DXVECTOR3 *pRayPosition,
        CONST D3DXVECTOR3 *pRayDirection);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeTangentFrame(ID3DXMesh *pMesh,
                                       DWORD dwOptions);


__declspec(dllimport)
HRESULT WINAPI D3DXComputeTangentFrameEx(ID3DXMesh *pMesh,
                                         DWORD dwTextureInSemantic,
                                         DWORD dwTextureInIndex,
                                         DWORD dwUPartialOutSemantic,
                                         DWORD dwUPartialOutIndex,
                                         DWORD dwVPartialOutSemantic,
                                         DWORD dwVPartialOutIndex,
                                         DWORD dwNormalOutSemantic,
                                         DWORD dwNormalOutIndex,
                                         DWORD dwOptions,
                                         CONST DWORD *pdwAdjacency,
                                         FLOAT fPartialEdgeThreshold,
                                         FLOAT fSingularPointThreshold,
                                         FLOAT fNormalEdgeThreshold,
                                         ID3DXMesh **ppMeshOut,
                                         ID3DXBuffer **ppVertexMapping);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeTangent(LPD3DXMESH Mesh,
                                 DWORD TexStage,
                                 DWORD TangentIndex,
                                 DWORD BinormIndex,
                                 DWORD Wrap,
                                 CONST DWORD *pAdjacency);

__declspec(dllimport)
HRESULT WINAPI D3DXUVAtlasCreate(LPD3DXMESH pMesh,
                                 UINT uMaxChartNumber,
                                 FLOAT fMaxStretch,
                                 UINT uWidth,
                                 UINT uHeight,
                                 FLOAT fGutter,
                                 DWORD dwTextureIndex,
                                 CONST DWORD *pdwAdjacency,
                                 CONST DWORD *pdwFalseEdgeAdjacency,
                                 CONST FLOAT *pfIMTArray,
                                 LPD3DXUVATLASCB pStatusCallback,
                                 FLOAT fCallbackFrequency,
                                 LPVOID pUserContext,
                                 DWORD dwOptions,
                                 LPD3DXMESH *ppMeshOut,
                                 LPD3DXBUFFER *ppFacePartitioning,
                                 LPD3DXBUFFER *ppVertexRemapArray,
                                 FLOAT *pfMaxStretchOut,
                                 UINT *puNumChartsOut);

__declspec(dllimport)
HRESULT WINAPI D3DXUVAtlasPartition(LPD3DXMESH pMesh,
                                    UINT uMaxChartNumber,
                                    FLOAT fMaxStretch,
                                    DWORD dwTextureIndex,
                                    CONST DWORD *pdwAdjacency,
                                    CONST DWORD *pdwFalseEdgeAdjacency,
                                    CONST FLOAT *pfIMTArray,
                                    LPD3DXUVATLASCB pStatusCallback,
                                    FLOAT fCallbackFrequency,
                                    LPVOID pUserContext,
                                    DWORD dwOptions,
                                    LPD3DXMESH *ppMeshOut,
                                    LPD3DXBUFFER *ppFacePartitioning,
                                    LPD3DXBUFFER *ppVertexRemapArray,
                                    LPD3DXBUFFER *ppPartitionResultAdjacency,
                                    FLOAT *pfMaxStretchOut,
                                    UINT *puNumChartsOut);

__declspec(dllimport)
HRESULT WINAPI D3DXUVAtlasPack(ID3DXMesh *pMesh,
                               UINT uWidth,
                               UINT uHeight,
                               FLOAT fGutter,
                               DWORD dwTextureIndex,
                               CONST DWORD *pdwPartitionResultAdjacency,
                               LPD3DXUVATLASCB pStatusCallback,
                               FLOAT fCallbackFrequency,
                               LPVOID pUserContext,
                               DWORD dwOptions,
                               LPD3DXBUFFER pFacePartitioning);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeIMTFromPerVertexSignal (
    LPD3DXMESH pMesh,
    CONST FLOAT *pfVertexSignal, // uSignalDimension floats per vertex
    UINT uSignalDimension,
    UINT uSignalStride,         // stride of signal in bytes
    DWORD dwOptions,            // reserved for future use
    LPD3DXUVATLASCB pStatusCallback,
    LPVOID pUserContext,
    LPD3DXBUFFER *ppIMTData);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeIMTFromSignal(
    LPD3DXMESH pMesh,
    DWORD dwTextureIndex,
    UINT uSignalDimension,
    FLOAT fMaxUVDistance,
    DWORD dwOptions, // reserved for future use
    LPD3DXIMTSIGNALCALLBACK pSignalCallback,
    VOID *pUserData,
    LPD3DXUVATLASCB pStatusCallback,
    LPVOID pUserContext,
    LPD3DXBUFFER *ppIMTData);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeIMTFromTexture(
    LPD3DXMESH pMesh,
    LPDIRECT3DTEXTURE9 pTexture,
    DWORD dwTextureIndex,
    DWORD dwOptions,
    LPD3DXUVATLASCB pStatusCallback,
    LPVOID pUserContext,
    LPD3DXBUFFER *ppIMTData);

__declspec(dllimport)
HRESULT WINAPI D3DXComputeIMTFromPerTexelSignal(
    LPD3DXMESH pMesh,
    DWORD dwTextureIndex,
    FLOAT *pfTexelSignal,
    UINT uWidth,
    UINT uHeight,
    UINT uSignalDimension,
    UINT uComponents,
    DWORD dwOptions,
    LPD3DXUVATLASCB pStatusCallback,
    LPVOID pUserContext,
    LPD3DXBUFFER *ppIMTData);

__declspec(dllimport)
HRESULT WINAPI
    D3DXConvertMeshSubsetToSingleStrip(
        LPD3DXBASEMESH MeshIn,
        DWORD AttribId,
        DWORD IBOptions,
        LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
        DWORD *pNumIndices);

__declspec(dllimport)
HRESULT WINAPI
    D3DXConvertMeshSubsetToStrips(
        LPD3DXBASEMESH MeshIn,
        DWORD AttribId,
        DWORD IBOptions,
        LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
        DWORD *pNumIndices,
        LPD3DXBUFFER *ppStripLengths,
        DWORD *pNumStrips);

__declspec(dllimport)
HRESULT WINAPI
    D3DXOptimizeFaces(
        LPCVOID pbIndices, 
        UINT cFaces, 
        UINT cVertices, 
        BOOL b32BitIndices, 
        DWORD* pFaceRemap);

__declspec(dllimport)
HRESULT WINAPI
    D3DXOptimizeVertices(
        LPCVOID pbIndices, 
        UINT cFaces, 
        UINT cVertices, 
        BOOL b32BitIndices, 
        DWORD* pVertexRemap);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreatePRTBuffer( 
        UINT NumSamples,
        UINT NumCoeffs,
        UINT NumChannels,
        LPD3DXPRTBUFFER* ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreatePRTBufferTex( 
        UINT Width,
        UINT Height,
        UINT NumCoeffs,
        UINT NumChannels,
        LPD3DXPRTBUFFER* ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadPRTBufferFromFileA(
        LPCSTR pFilename, 
        LPD3DXPRTBUFFER*       ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadPRTBufferFromFileW(
        LPCWSTR pFilename, 
        LPD3DXPRTBUFFER*       ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSavePRTBufferToFileA(
        LPCSTR pFileName,
        LPD3DXPRTBUFFER pBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSavePRTBufferToFileW(
        LPCWSTR pFileName,
        LPD3DXPRTBUFFER pBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadPRTCompBufferFromFileA(
        LPCSTR pFilename, 
        LPD3DXPRTCOMPBUFFER*       ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadPRTCompBufferFromFileW(
        LPCWSTR pFilename, 
        LPD3DXPRTCOMPBUFFER*       ppBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSavePRTCompBufferToFileA(
        LPCSTR pFileName,
        LPD3DXPRTCOMPBUFFER pBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSavePRTCompBufferToFileW(
        LPCWSTR pFileName,
        LPD3DXPRTCOMPBUFFER pBuffer);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreatePRTCompBuffer(
        D3DXSHCOMPRESSQUALITYTYPE Quality,
        UINT NumClusters, 
        UINT NumPCA,
        LPD3DXSHPRTSIMCB pCB,
        LPVOID lpUserContext,        
        LPD3DXPRTBUFFER  pBufferIn,
        LPD3DXPRTCOMPBUFFER *ppBufferOut
    );

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateTextureGutterHelper( 
        UINT Width,
        UINT Height,
        LPD3DXMESH pMesh, 
        FLOAT GutterSize,
        LPD3DXTEXTUREGUTTERHELPER* ppBuffer);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreatePRTEngine( 
        LPD3DXMESH pMesh,
        DWORD *pAdjacency,
        BOOL ExtractUVs,
        LPD3DXMESH pBlockerMesh, 
        LPD3DXPRTENGINE* ppEngine);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXConcatenateMeshes(
        LPD3DXMESH *ppMeshes, 
        UINT NumMeshes, 
        DWORD Options, 
        CONST D3DXMATRIX *pGeomXForms, 
        CONST D3DXMATRIX *pTextureXForms, 
        CONST D3DVERTEXELEMENT9 *pDecl,
        LPDIRECT3DDEVICE9 pD3DDevice, 
        LPD3DXMESH *ppMeshOut);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXSHPRTCompSuperCluster(
        UINT *pClusterIDs, 
        LPD3DXMESH pScene, 
        UINT MaxNumClusters, 
        UINT NumClusters,
        UINT *pSuperClusterIDs, 
        UINT *pNumSuperClusters);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXSHPRTCompSplitMeshSC(
        UINT *pClusterIDs, 
        UINT NumVertices, 
        UINT NumClusters, 
        UINT *pSuperClusterIDs, 
        UINT NumSuperClusters,
        LPVOID pInputIB, 
        BOOL InputIBIs32Bit, 
        UINT NumFaces,
        LPD3DXBUFFER *ppIBData, 
        UINT *pIBDataLength, 
        BOOL OutputIBIs32Bit, 
        LPD3DXBUFFER *ppFaceRemap, 
        LPD3DXBUFFER *ppVertData, 
        UINT *pVertDataLength, 
        UINT *pSCClusterList,
        D3DXSHPRTSPLITMESHCLUSTERDATA *pSCData);

__declspec(dllimport)
HRESULT WINAPI
    D3DXAssembleShaderFromFileA(
        LPCSTR                          pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXAssembleShaderFromFileW(
        LPCWSTR                         pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXAssembleShaderFromResourceA(
        HMODULE                         hSrcModule,
        LPCSTR                          pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXAssembleShaderFromResourceW(
        HMODULE                         hSrcModule,
        LPCWSTR                         pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXAssembleShader(
        LPCSTR                          pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCompileShaderFromFileA(
        LPCSTR                          pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCompileShaderFromFileW(
        LPCWSTR                         pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCompileShaderFromResourceA(
        HMODULE                         hSrcModule,
        LPCSTR                          pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCompileShaderFromResourceW(
        HMODULE                         hSrcModule,
        LPCWSTR                         pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCompileShader(
        LPCSTR                          pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pFunctionName,
        LPCSTR                          pProfile,
        DWORD                           Flags,
        LPD3DXBUFFER*                   ppShader,
        LPD3DXBUFFER*                   ppErrorMsgs,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXDisassembleShader(
        CONST DWORD*                    pShader, 
        BOOL                            EnableColorCode, 
        LPCSTR                          pComments, 
        LPD3DXBUFFER*                   ppDisassembly);

__declspec(dllimport)
LPCSTR WINAPI
    D3DXGetPixelShaderProfile(
        LPDIRECT3DDEVICE9               pDevice);

__declspec(dllimport)
LPCSTR WINAPI
    D3DXGetVertexShaderProfile(
        LPDIRECT3DDEVICE9               pDevice);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFindShaderComment(
        CONST DWORD*                    pFunction,
        DWORD                           FourCC,
        LPCVOID*                        ppData,
        UINT*                           pSizeInBytes);

__declspec(dllimport)
UINT WINAPI
    D3DXGetShaderSize(
        CONST DWORD*                    pFunction);

__declspec(dllimport)
DWORD WINAPI
    D3DXGetShaderVersion(
        CONST DWORD*                    pFunction);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetShaderInputSemantics(
        CONST DWORD*                    pFunction,
        D3DXSEMANTIC*                   pSemantics,
        UINT*                           pCount);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetShaderOutputSemantics(
        CONST DWORD*                    pFunction,
        D3DXSEMANTIC*                   pSemantics,
        UINT*                           pCount);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetShaderSamplers(
        CONST DWORD*                    pFunction,
        LPCSTR*                         pSamplers,
        UINT*                           pCount);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetShaderConstantTable(
        CONST DWORD*                    pFunction,
        LPD3DXCONSTANTTABLE*            ppConstantTable);


__declspec(dllimport)
HRESULT WINAPI
    D3DXGetShaderConstantTableEx(
        CONST DWORD*                    pFunction,
        DWORD                           Flags,
        LPD3DXCONSTANTTABLE*            ppConstantTable);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureShader(
        CONST DWORD*                    pFunction, 
        LPD3DXTEXTURESHADER*            ppTextureShader);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXPreprocessShaderFromFileA(
        LPCSTR                       pSrcFile,
        CONST D3DXMACRO*             pDefines,
        LPD3DXINCLUDE                pInclude,
        LPD3DXBUFFER*                ppShaderText,
        LPD3DXBUFFER*                ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXPreprocessShaderFromFileW(
        LPCWSTR                      pSrcFile,
        CONST D3DXMACRO*             pDefines,
        LPD3DXINCLUDE                pInclude,
        LPD3DXBUFFER*                ppShaderText,
        LPD3DXBUFFER*                ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXPreprocessShaderFromResourceA(
        HMODULE                      hSrcModule,
        LPCSTR                       pSrcResource,
        CONST D3DXMACRO*             pDefines,
        LPD3DXINCLUDE                pInclude,
        LPD3DXBUFFER*                ppShaderText,
        LPD3DXBUFFER*                ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXPreprocessShaderFromResourceW(
        HMODULE                      hSrcModule,
        LPCWSTR                      pSrcResource,
        CONST D3DXMACRO*             pDefines,
        LPD3DXINCLUDE                pInclude,
        LPD3DXBUFFER*                ppShaderText,
        LPD3DXBUFFER*                ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXPreprocessShader(
        LPCSTR                       pSrcData,
        UINT                         SrcDataSize,
        CONST D3DXMACRO*             pDefines,
        LPD3DXINCLUDE                pInclude,
        LPD3DXBUFFER*                ppShaderText,
        LPD3DXBUFFER*                ppErrorMsgs);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectPool(
        LPD3DXEFFECTPOOL*               ppPool);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromFileA(
        LPDIRECT3DDEVICE9               pDevice,
        LPCSTR                          pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromFileW(
        LPDIRECT3DDEVICE9               pDevice,
        LPCWSTR                         pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromResourceA(
        LPDIRECT3DDEVICE9               pDevice,
        HMODULE                         hSrcModule,
        LPCSTR                          pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromResourceW(
        LPDIRECT3DDEVICE9               pDevice,
        HMODULE                         hSrcModule,
        LPCWSTR                         pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffect(
        LPDIRECT3DDEVICE9               pDevice,
        LPCVOID                         pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromFileExA(
        LPDIRECT3DDEVICE9               pDevice,
        LPCSTR                          pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pSkipConstants, 
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromFileExW(
        LPDIRECT3DDEVICE9               pDevice,
        LPCWSTR                         pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pSkipConstants, 
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromResourceExA(
        LPDIRECT3DDEVICE9               pDevice,
        HMODULE                         hSrcModule,
        LPCSTR                          pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pSkipConstants, 
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectFromResourceExW(
        LPDIRECT3DDEVICE9               pDevice,
        HMODULE                         hSrcModule,
        LPCWSTR                         pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pSkipConstants, 
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectEx(
        LPDIRECT3DDEVICE9               pDevice,
        LPCVOID                         pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        LPCSTR                          pSkipConstants, 
        DWORD                           Flags,
        LPD3DXEFFECTPOOL                pPool,
        LPD3DXEFFECT*                   ppEffect,
        LPD3DXBUFFER*                   ppCompilationErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectCompilerFromFileA(
        LPCSTR                          pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTCOMPILER*           ppCompiler,
        LPD3DXBUFFER*                   ppParseErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectCompilerFromFileW(
        LPCWSTR                         pSrcFile,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTCOMPILER*           ppCompiler,
        LPD3DXBUFFER*                   ppParseErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectCompilerFromResourceA(
        HMODULE                         hSrcModule,
        LPCSTR                          pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTCOMPILER*           ppCompiler,
        LPD3DXBUFFER*                   ppParseErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectCompilerFromResourceW(
        HMODULE                         hSrcModule,
        LPCWSTR                         pSrcResource,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTCOMPILER*           ppCompiler,
        LPD3DXBUFFER*                   ppParseErrors);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateEffectCompiler(
        LPCSTR                          pSrcData,
        UINT                            SrcDataLen,
        CONST D3DXMACRO*                pDefines,
        LPD3DXINCLUDE                   pInclude,
        DWORD                           Flags,
        LPD3DXEFFECTCOMPILER*           ppCompiler,
        LPD3DXBUFFER*                   ppParseErrors);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXDisassembleEffect(
        LPD3DXEFFECT pEffect, 
        BOOL EnableColorCode, 
        LPD3DXBUFFER *ppDisassembly);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetImageInfoFromFileA(
        LPCSTR                    pSrcFile,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetImageInfoFromFileW(
        LPCWSTR                   pSrcFile,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetImageInfoFromResourceA(
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetImageInfoFromResourceW(
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXGetImageInfoFromFileInMemory(
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromFileA(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCSTR                    pSrcFile,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromFileW(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCWSTR                   pSrcFile,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromResourceA(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromResourceW(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromFileInMemory(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromSurface(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadSurfaceFromMemory(
        LPDIRECT3DSURFACE9        pDestSurface,
        CONST PALETTEENTRY*       pDestPalette,
        CONST RECT*               pDestRect,
        LPCVOID                   pSrcMemory,
        D3DFORMAT                 SrcFormat,
        UINT                      SrcPitch,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveSurfaceToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveSurfaceToFileW(
        LPCWSTR                   pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveSurfaceToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DSURFACE9        pSrcSurface,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST RECT*               pSrcRect);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromFileA(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCSTR                    pSrcFile,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromFileW(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCWSTR                   pSrcFile,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromResourceA(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromResourceW(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromFileInMemory(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromVolume(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

__declspec(dllimport)
HRESULT WINAPI
    D3DXLoadVolumeFromMemory(
        LPDIRECT3DVOLUME9         pDestVolume,
        CONST PALETTEENTRY*       pDestPalette,
        CONST D3DBOX*             pDestBox,
        LPCVOID                   pSrcMemory,
        D3DFORMAT                 SrcFormat,
        UINT                      SrcRowPitch,
        UINT                      SrcSlicePitch,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox,
        DWORD                     Filter,
        D3DCOLOR                  ColorKey);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveVolumeToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveVolumeToFileW(
        LPCWSTR                   pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveVolumeToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DVOLUME9         pSrcVolume,
        CONST PALETTEENTRY*       pSrcPalette,
        CONST D3DBOX*             pSrcBox);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCheckTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pWidth,
        UINT*                     pHeight,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCheckCubeTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pSize,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCheckVolumeTextureRequirements(
        LPDIRECT3DDEVICE9         pDevice,
        UINT*                     pWidth,
        UINT*                     pHeight,
        UINT*                     pDepth,
        UINT*                     pNumMipLevels,
        DWORD                     Usage,
        D3DFORMAT*                pFormat,
        D3DPOOL                   Pool);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTexture(
        LPDIRECT3DDEVICE9         pDevice,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromResourceW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileExW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileExW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileExA(
        LPDIRECT3DDEVICE9         pDevice,
        LPCSTR                    pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileExW(
        LPDIRECT3DDEVICE9         pDevice,
        LPCWSTR                   pSrcFile,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromResourceExW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromResourceExW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceExA(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCSTR                    pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromResourceExW(
        LPDIRECT3DDEVICE9         pDevice,
        HMODULE                   hSrcModule,
        LPCWSTR                   pSrcResource,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileInMemory(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Width,
        UINT                      Height,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DTEXTURE9*       ppTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateCubeTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Size,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateVolumeTextureFromFileInMemoryEx(
        LPDIRECT3DDEVICE9         pDevice,
        LPCVOID                   pSrcData,
        UINT                      SrcDataSize,
        UINT                      Width,
        UINT                      Height,
        UINT                      Depth,
        UINT                      MipLevels,
        DWORD                     Usage,
        D3DFORMAT                 Format,
        D3DPOOL                   Pool,
        DWORD                     Filter,
        DWORD                     MipFilter,
        D3DCOLOR                  ColorKey,
        D3DXIMAGE_INFO*           pSrcInfo,
        PALETTEENTRY*             pPalette,
        LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveTextureToFileA(
        LPCSTR                    pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DBASETEXTURE9    pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveTextureToFileW(
        LPCWSTR                   pDestFile,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DBASETEXTURE9    pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette);

__declspec(dllimport)
HRESULT WINAPI
    D3DXSaveTextureToFileInMemory(
        LPD3DXBUFFER*             ppDestBuf,
        D3DXIMAGE_FILEFORMAT      DestFormat,
        LPDIRECT3DBASETEXTURE9    pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFilterTexture(
        LPDIRECT3DBASETEXTURE9    pBaseTexture,
        CONST PALETTEENTRY*       pPalette,
        UINT                      SrcLevel,
        DWORD                     Filter);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFillTexture(
        LPDIRECT3DTEXTURE9        pTexture,
        LPD3DXFILL2D              pFunction,
        LPVOID                    pData);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFillCubeTexture(
        LPDIRECT3DCUBETEXTURE9    pCubeTexture,
        LPD3DXFILL3D              pFunction,
        LPVOID                    pData);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFillVolumeTexture(
        LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
        LPD3DXFILL3D              pFunction,
        LPVOID                    pData);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXFillTextureTX(
        LPDIRECT3DTEXTURE9        pTexture,
        LPD3DXTEXTURESHADER       pTextureShader);

__declspec(dllimport)
HRESULT WINAPI
    D3DXFillCubeTextureTX(
        LPDIRECT3DCUBETEXTURE9    pCubeTexture,
        LPD3DXTEXTURESHADER       pTextureShader);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXFillVolumeTextureTX(
        LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
        LPD3DXTEXTURESHADER       pTextureShader);

__declspec(dllimport)
HRESULT WINAPI
    D3DXComputeNormalMap(
        LPDIRECT3DTEXTURE9        pTexture,
        LPDIRECT3DTEXTURE9        pSrcTexture,
        CONST PALETTEENTRY*       pSrcPalette,
        DWORD                     Flags,
        DWORD                     Channel,
        FLOAT                     Amplitude);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreatePolygon(
        LPDIRECT3DDEVICE9   pDevice,
        FLOAT               Length, 
        UINT                Sides, 
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateBox(
        LPDIRECT3DDEVICE9   pDevice, 
        FLOAT               Width,
        FLOAT               Height,
        FLOAT               Depth,
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI 
    D3DXCreateCylinder(
        LPDIRECT3DDEVICE9   pDevice,
        FLOAT               Radius1, 
        FLOAT               Radius2, 
        FLOAT               Length, 
        UINT                Slices, 
        UINT                Stacks,   
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateSphere(
        LPDIRECT3DDEVICE9  pDevice, 
        FLOAT              Radius, 
        UINT               Slices, 
        UINT               Stacks,
        LPD3DXMESH*        ppMesh,
        LPD3DXBUFFER*      ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTorus(
        LPDIRECT3DDEVICE9   pDevice,
        FLOAT               InnerRadius,
        FLOAT               OuterRadius, 
        UINT                Sides,
        UINT                Rings, 
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTeapot(
        LPDIRECT3DDEVICE9   pDevice,
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextA(
        LPDIRECT3DDEVICE9   pDevice,
        HDC                 hDC,
        LPCSTR              pText,
        FLOAT               Deviation,
        FLOAT               Extrusion,
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency,
        LPGLYPHMETRICSFLOAT pGlyphMetrics);

__declspec(dllimport)
HRESULT WINAPI
    D3DXCreateTextW(
        LPDIRECT3DDEVICE9   pDevice,
        HDC                 hDC,
        LPCWSTR             pText,
        FLOAT               Deviation,
        FLOAT               Extrusion,
        LPD3DXMESH*         ppMesh,
        LPD3DXBUFFER*       ppAdjacency,
        LPGLYPHMETRICSFLOAT pGlyphMetrics);

__declspec(dllimport)
HRESULT WINAPI 
D3DXLoadMeshHierarchyFromXA
    (
    LPCSTR Filename,
    DWORD MeshOptions,
    LPDIRECT3DDEVICE9 pD3DDevice,
    LPD3DXALLOCATEHIERARCHY pAlloc,
    LPD3DXLOADUSERDATA pUserDataLoader, 
    LPD3DXFRAME *ppFrameHierarchy,
    LPD3DXANIMATIONCONTROLLER *ppAnimController
    );

__declspec(dllimport)
HRESULT WINAPI 
D3DXLoadMeshHierarchyFromXW
    (
    LPCWSTR Filename,
    DWORD MeshOptions,
    LPDIRECT3DDEVICE9 pD3DDevice,
    LPD3DXALLOCATEHIERARCHY pAlloc,
    LPD3DXLOADUSERDATA pUserDataLoader, 
    LPD3DXFRAME *ppFrameHierarchy,
    LPD3DXANIMATIONCONTROLLER *ppAnimController
    );

__declspec(dllimport)
HRESULT WINAPI 
D3DXLoadMeshHierarchyFromXInMemory
    (
    LPCVOID Memory,
    DWORD SizeOfMemory,
    DWORD MeshOptions,
    LPDIRECT3DDEVICE9 pD3DDevice,
    LPD3DXALLOCATEHIERARCHY pAlloc,
    LPD3DXLOADUSERDATA pUserDataLoader, 
    LPD3DXFRAME *ppFrameHierarchy,
    LPD3DXANIMATIONCONTROLLER *ppAnimController
    );

__declspec(dllimport)
HRESULT WINAPI 
D3DXSaveMeshHierarchyToFileA
    (
    LPCSTR Filename,
    DWORD XFormat,
    CONST D3DXFRAME *pFrameRoot, 
    LPD3DXANIMATIONCONTROLLER pAnimcontroller,
    LPD3DXSAVEUSERDATA pUserDataSaver
    );

__declspec(dllimport)
HRESULT WINAPI 
D3DXSaveMeshHierarchyToFileW
    (
    LPCWSTR Filename,
    DWORD XFormat,
    CONST D3DXFRAME *pFrameRoot, 
    LPD3DXANIMATIONCONTROLLER pAnimController,
    LPD3DXSAVEUSERDATA pUserDataSaver
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXFrameDestroy
    (
    LPD3DXFRAME pFrameRoot,
    LPD3DXALLOCATEHIERARCHY pAlloc
    );

__declspec(dllimport)
HRESULT WINAPI 
D3DXFrameAppendChild
    (
    LPD3DXFRAME pFrameParent,
    CONST D3DXFRAME *pFrameChild
    );

__declspec(dllimport)
LPD3DXFRAME WINAPI 
D3DXFrameFind
    (
    CONST D3DXFRAME *pFrameRoot,
    LPCSTR Name
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXFrameRegisterNamedMatrices
    (
    LPD3DXFRAME pFrameRoot,
    LPD3DXANIMATIONCONTROLLER pAnimController
    );

__declspec(dllimport)
UINT WINAPI
D3DXFrameNumNamedMatrices
    (
    CONST D3DXFRAME *pFrameRoot
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXFrameCalculateBoundingSphere
    (
    CONST D3DXFRAME *pFrameRoot,       
    LPD3DXVECTOR3 pObjectCenter,  
    FLOAT *pObjectRadius          
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXCreateKeyframedAnimationSet
    (
    LPCSTR pName,                               
    DOUBLE TicksPerSecond,
    D3DXPLAYBACK_TYPE Playback,
    UINT NumAnimations,                         
    UINT NumCallbackKeys,                       
    CONST D3DXKEY_CALLBACK *pCallbackKeys,           
    LPD3DXKEYFRAMEDANIMATIONSET *ppAnimationSet 
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXCreateCompressedAnimationSet
    (
    LPCSTR pName,                               
    DOUBLE TicksPerSecond,
    D3DXPLAYBACK_TYPE Playback,
    LPD3DXBUFFER pCompressedData,                         
    UINT NumCallbackKeys,                       
    CONST D3DXKEY_CALLBACK *pCallbackKeys,           
    LPD3DXCOMPRESSEDANIMATIONSET *ppAnimationSet 
    );

__declspec(dllimport)
HRESULT WINAPI
D3DXCreateAnimationController
    (
    UINT MaxNumMatrices,                       
    UINT MaxNumAnimationSets,                  
    UINT MaxNumTracks,                         
    UINT MaxNumEvents,                         
    LPD3DXANIMATIONCONTROLLER *ppAnimController
    );
}
#endif