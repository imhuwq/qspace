#ifndef QSPACE_TEXTUREDATA_H
#define QSPACE_TEXTUREDATA_H

#include <QSharedPointer>
#include "TextureFile.h"

class TextureData {
public:
    enum BlendMode {
        Translucent,
        Additive,
        Modulate,
        Modulate2,
        Over,
        Normal,
        Dissolve,
        Darken,
        ColorBurn,
        LinearBurn,
        DarkerColor,
        Lighten,
        Screen,
        ColorDodge,
        LinearDodge,
        LighterColor,
        SoftLight,
        HardLight,
        VividLight,
        LinearLight,
        PinLight,
        HardMix,
        Difference,
        Exclusion,
        Substract,
        Divide,
        Hue,
        Saturation,
        Color,
        Luminosity,
        Overlay,
        None
    };
    explicit TextureData(const QString &channel) : m_channel(channel) {}

    QString channel() { return m_channel; }

    void setName(const QString &name) { m_name = name; }

    QString name() { return m_name; }

    void setTextureFile(QSharedPointer<TextureFile> texture) { m_textureFile = texture; }

    QSharedPointer<TextureFile> getTextureFile() { return m_textureFile; }

    void setScaleU(float scaleU) { m_scaleU = scaleU; }

    float scaleU() { return m_scaleU; }

    void setScaleV(float scaleV) { m_scaleV = scaleV; }

    float scaleV() { return m_scaleV; }

    void setTranslationU(float translationU) { m_translationU = translationU; }

    float translationU() { return m_translationU; }

    void setTranslationV(float translationV) { m_translationV = translationV; }

    float translationV() { return m_translationV; }

    void setSwapUV(bool swapUV) { m_swapUV = swapUV; }

    bool swapUV() { return m_swapUV; }

    void setRotationU(float rotationU) { m_rotationU = rotationU; }

    float rotationU() { return m_rotationU; }

    void setRotationV(float rotationV) { m_rotationV = rotationV; }

    float rotationV() { return m_rotationV; }

    void setRotationW(float rotationW) { m_rotationW = rotationW; }

    float rotationW() { return m_rotationW; }

    void setCroppingLeft(int croppingLeft) { m_croppingLeft = croppingLeft; }

    int croppingLeft() { return m_croppingLeft; }

    void setCroppingRight(int croppingRight) { m_croppingRight = croppingRight; }

    int croppingRight() { return m_croppingRight; }

    void setCroppingTop(int croppingTop) { m_croppingTop = croppingTop; }

    int croppingTop() { return m_croppingTop; }

    void setCroppingBottom(int croppingBottom) { m_croppingBottom = croppingBottom; }

    int croppingBottom() { return m_croppingBottom; }

    void setBlendMode(int blendMode) { m_blendMode = blendMode; }

    int blendMode() { return m_blendMode; }

private:
    QSharedPointer<TextureFile> m_textureFile = QSharedPointer<TextureFile>(nullptr);
    QString m_channel = "";
    QString m_name = "";
    float m_scaleU = 0.0f;
    float m_scaleV = 0.0f;
    float m_translationU = 0.0f;
    float m_translationV = 0.0f;
    bool m_swapUV = false;
    float m_rotationU = 0.0f;
    float m_rotationV = 0.0f;
    float m_rotationW = 0.0f;
    int m_croppingLeft = 0;
    int m_croppingRight = 0;
    int m_croppingTop = 0;
    int m_croppingBottom = 0;
    int m_blendMode = -1;
};

#endif //QSPACE_TEXTUREDATA_H
