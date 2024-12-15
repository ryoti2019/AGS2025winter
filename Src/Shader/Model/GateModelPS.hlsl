// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@:�X���b�g4�Ԗ�
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

Texture2D noise : register(t1); //�e�N�X�`��
SamplerState noiseMapSampler : register(s1);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    
    // UV���W���擾
    float2 uv = PSInput.uv;

    // ���ԃx�[�X�ŗh���������
    uv.y += sin(g_time + uv.x * 10.0f) * 0.2f; // x���W�Ɋ�Â���y�����ɗh��

    // �m�C�Y�e�N�X�`�����T���v�����O
    float4 noiseColor = noise.Sample(noiseMapSampler, uv);

    // �ǂ̐F��Ԃɐݒ�
    float4 wallColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // �ԐF (R=1, G=0, B=0, A=1)

    // �m�C�Y���ʂ�F�ɉ�����
    wallColor.rgb += noiseColor.rgb * 1.0f;

    // �����x��������i�m�C�Y�Ǝ��ԂɊ�Â��j
    wallColor.a *= 0.8f + sin(g_time * 0.5f) * 0.2f; // �����x�̒����i���Ԃŕω��j

    // **���Ԃɂ�铧������������i���X�ɓ������j**
    wallColor.a *= saturate(1.0f - g_alpha); // ���Ԃ��o�ɂ�ē����x��������

    return wallColor;
    
}