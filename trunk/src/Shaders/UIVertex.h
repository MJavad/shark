#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.16384
//
///
// Parameters:
//
//   float4x4 g_orthoMatrix;
//
//
// Registers:
//
//   Name          Reg   Size
//   ------------- ----- ----
//   g_orthoMatrix c0       4
//

    vs_3_0
    def c4, 1, 0, -1, 0
    dcl_position v0
    dcl_texcoord v1
    dcl_color v2
    dcl_position o0
    dcl_texcoord o1.xy
    dcl_texcoord1 o2
    mad r0.xyz, v0.xyxw, c4.xxyw, c4.yyxw
    dp3 o0.z, r0, c2.xyww
    dp3 o0.w, r0, c3.xyww
    dp3 r0.w, r0, c0.xyww
    dp3 r0.x, r0, c1.xyww
    add o0.y, -r0.x, c4.x
    add o0.x, r0.w, c4.z
    mov o1.xy, v1
    mov o2, v2

// approximately 9 instruction slots used
#endif

const BYTE UIVertexShader[] =
{
      0,   3, 254, 255, 254, 255, 
     36,   0,  67,  84,  65,  66, 
     28,   0,   0,   0,  87,   0, 
      0,   0,   0,   3, 254, 255, 
      1,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
     80,   0,   0,   0,  48,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   0,   2,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
    103,  95, 111, 114, 116, 104, 
    111,  77,  97, 116, 114, 105, 
    120,   0, 171, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 118, 115,  95,  51, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  57,  46,  51,  48, 
     46,  57,  50,  48,  48,  46, 
     49,  54,  51,  56,  52,   0, 
    171, 171,  81,   0,   0,   5, 
      4,   0,  15, 160,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0, 128, 191,   0,   0, 
      0,   0,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
     15, 144,  31,   0,   0,   2, 
      5,   0,   0, 128,   1,   0, 
     15, 144,  31,   0,   0,   2, 
     10,   0,   0, 128,   2,   0, 
     15, 144,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
     15, 224,  31,   0,   0,   2, 
      5,   0,   0, 128,   1,   0, 
      3, 224,  31,   0,   0,   2, 
      5,   0,   1, 128,   2,   0, 
     15, 224,   4,   0,   0,   4, 
      0,   0,   7, 128,   0,   0, 
    196, 144,   4,   0, 208, 160, 
      4,   0, 197, 160,   8,   0, 
      0,   3,   0,   0,   4, 224, 
      0,   0, 228, 128,   2,   0, 
    244, 160,   8,   0,   0,   3, 
      0,   0,   8, 224,   0,   0, 
    228, 128,   3,   0, 244, 160, 
      8,   0,   0,   3,   0,   0, 
      8, 128,   0,   0, 228, 128, 
      0,   0, 244, 160,   8,   0, 
      0,   3,   0,   0,   1, 128, 
      0,   0, 228, 128,   1,   0, 
    244, 160,   2,   0,   0,   3, 
      0,   0,   2, 224,   0,   0, 
      0, 129,   4,   0,   0, 160, 
      2,   0,   0,   3,   0,   0, 
      1, 224,   0,   0, 255, 128, 
      4,   0, 170, 160,   1,   0, 
      0,   2,   1,   0,   3, 224, 
      1,   0, 228, 144,   1,   0, 
      0,   2,   2,   0,  15, 224, 
      2,   0, 228, 144, 255, 255, 
      0,   0
};
