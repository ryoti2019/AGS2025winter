// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ:スロット4番目
cbuffer cbParam : register(b4)
{
    float4 g_diff_color;
    float4 g_ambient_color;
    float3 g_light_dir;
    float g_time;
    float g_alpha;
    float g_noise;
    float2 dummy;
}

Texture2D noise : register(t1); //テクスチャ
SamplerState noiseMapSampler : register(s1);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    
    // UV座標を取得
    float2 uv = PSInput.uv;

    // 時間ベースで揺れを加える
    uv.y += sin(g_time + uv.x * 10.0f) * 0.2f; // x座標に基づいてy方向に揺れ

    // ノイズテクスチャをサンプリング
    float4 noiseColor = noise.Sample(noiseMapSampler, uv);

    // 壁の色を赤に設定
    float4 wallColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // 赤色 (R=1, G=0, B=0, A=1)

    // ノイズ効果を色に加える
    wallColor.rgb += noiseColor.rgb * 1.0f;

    // 透明度を加える（ノイズと時間に基づく）
    wallColor.a *= 0.8f + sin(g_time * 0.5f) * 0.2f; // 透明度の調整（時間で変化）

    // **時間による透明化を加える（徐々に透明化）**
    wallColor.a *= saturate(1.0f - g_alpha); // 時間が経つにつれて透明度が下がる

    return wallColor;
    
}