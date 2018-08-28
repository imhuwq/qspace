#ifndef QSPACE_TEXTURE_H
#define QSPACE_TEXTURE_H

#include <QSharedPointer>

using namespace std;

class Texture;

typedef QSharedPointer<Texture> TexturePtr;

typedef QSharedPointer<const Texture> kTexturePtr;

enum TextureBlendMode {
    kTranslucent = 0,
    kAdditive,
    kModulate,
    kModulate2,
    kOver,
    kNormal,
    kDissolve,
    kDarken,
    kColorBurn,
    kLinearBurn,
    kDarkerColor,
    kLighten,
    kScreen,
    kColorDodge,
    kLinearDodge,
    kLighterColor,
    kSoftLight,
    kHardLight,
    kVividLight,
    kLinearLight,
    kPinLight,
    kHardMix,
    kDifference,
    kExclusion,
    kSubstract,
    kDivide,
    kHue,
    kSaturation,
    kColor,
    kLuminosity,
    kOverlay,
    kNone,
    kBlendModeCount
};

class Texture {
public:
    explicit Texture(QString channel) : channel_(move(channel)) {}

    QString GetChannel() { return channel_; }

    void SetName(const QString &name) { name_ = name; }

    QString GetName() { return name_; }

    void SetTextureFile(const QString &texture) { texture_file_ = texture; }

    const QString &GetTextureFile() const { return texture_file_; }

    float GetScaleU() { return scale_u_; }

    void SetScaleU(float scaleU) { scale_u_ = scaleU; }

    float GetScaleV() { return scale_v_; }

    void SetScaleV(float scaleV) { scale_v_ = scaleV; }

    float GetTranslationU() { return translation_u_; }

    void SetTranslationU(float translationU) { translation_u_ = translationU; }

    float GetTranslationV() { return translation_v_; }

    void SetTranslationV(float translationV) { translation_v_ = translationV; }

    bool GetSwapUV() { return swap_uv_; }

    void SetSwapUV(bool swapUV) { swap_uv_ = swapUV; }

    float GetRotationU() { return rotation_u_; }

    void SetRotationU(float rotationU) { rotation_u_ = rotationU; }

    float GetRotationV() { return rotation_v_; }

    void SetRotationV(float rotationV) { rotation_v_ = rotationV; }

    float GetRotationW() { return rotation_w_; }

    void SetRotationW(float rotationW) { rotation_w_ = rotationW; }

    int GetCroppingLeft() { return cropping_left_; }

    void SetCroppingLeft(int croppingLeft) { cropping_left_ = croppingLeft; }

    int GetCroppingRight() { return cropping_right_; }

    void SetCroppingRight(int croppingRight) { cropping_right_ = croppingRight; }

    int GetCroppingTop() { return cropping_top_; }

    void SetCroppingTop(int croppingTop) { cropping_top_ = croppingTop; }

    int GetCroppingBottom() { return cropping_bottom_; }

    void SetCroppingBottom(int croppingBottom) { cropping_bottom_ = croppingBottom; }

    TextureBlendMode GetBlendMode() { return blend_mode_; }

    void SetBlendMode(TextureBlendMode blend_mode) { blend_mode_ = blend_mode; }

private:
    QString channel_ = "";
    QString name_ = "";
    QString texture_file_ = "";
    float scale_u_ = 0.0f;
    float scale_v_ = 0.0f;
    float translation_u_ = 0.0f;
    float translation_v_ = 0.0f;
    bool swap_uv_ = false;
    float rotation_u_ = 0.0f;
    float rotation_v_ = 0.0f;
    float rotation_w_ = 0.0f;
    int cropping_left_ = 0;
    int cropping_right_ = 0;
    int cropping_top_ = 0;
    int cropping_bottom_ = 0;
    TextureBlendMode blend_mode_ = TextureBlendMode::kNone;
};

#endif //QSPACE_TEXTURE_H
