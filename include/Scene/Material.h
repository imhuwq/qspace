#ifndef QSPACE_MATERIAL_H
#define QSPACE_MATERIAL_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QSharedPointer>
#include "TextureFile.h"
#include "TextureData.h"

class Material {
public:
    enum Shading {
        Phone = 1,
        Lambert = 2
    };

    Material(const QString &material_name) : m_name(material_name) {}

    const QString &name() const { return m_name; }

    const Shading &shading() const { return m_shading; }

    void setShading(const Shading shading) { m_shading = shading; }

    const QVector<double> &ambient() const { return m_ambient; }

    void setAmbient(const QVector<double> &ambient) { m_ambient = ambient; }

    const QVector<double> &diffuse() const { return m_diffuse; }

    void setDiffuse(const QVector<double> &diffuse) { m_diffuse = diffuse; }

    const QVector<double> &emissive() const { return m_emissive; }

    void setEmissive(const QVector<double> &emissive) { m_emissive = emissive; }

    const QVector<double> &specular() const { return m_specular; }

    void setSpecular(const QVector<double> &specular) { m_specular = specular; }

    float opacity() const { return m_opacity; }

    void setOpacity(const float &opacity) { m_opacity = opacity; }

    float shininess() const { return m_shininess; }

    void setShininess(float shininess) { m_shininess = shininess; }

    float reflectivity() { return m_reflectivity; }

    void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; }

    void addTexture(QSharedPointer<TextureData> textureData) { textures[textureData->channel()] = textureData; }

    QSharedPointer<TextureData> getTexture(const QString &channel) { return textures[channel]; }

private:
    QString m_name;
    Shading m_shading;
    QVector<double> m_ambient;
    QVector<double> m_diffuse;
    QVector<double> m_specular;
    QVector<double> m_emissive;
    float m_opacity = 0.0f;
    float m_shininess = 0.0f;
    float m_reflectivity = 0.0f;
    QMap<QString, QSharedPointer<TextureData>> textures;
};

#endif //QSPACE_MATERIAL_H
