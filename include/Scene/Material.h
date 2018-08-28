#ifndef QSPACE_MATERIAL_H
#define QSPACE_MATERIAL_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QSharedPointer>
#include "Texture.h"

using namespace std;

class Material;

typedef QSharedPointer<Material> MaterialPtr;

typedef QSharedPointer<const Material> kMaterialPtr;

class Material {
public:
    enum Shading {
        kPhone = 0,
        kLambert,
        kCount
    };

    explicit Material(QString name) : name_(move(name)), shading_(Shading::kPhone) {}

    const QString &GetName() const { return name_; }

    const Shading GetShading() const { return shading_; }

    void SetShading(const Shading shading) { shading_ = shading; }

    const QVector<double> &GetAmbient() const { return ambient_; }

    void SetAmbient(const QVector<double> &ambient) { ambient_ = ambient; }

    const QVector<double> &SetDiffuse() const { return diffuse_; }

    void SetDiffuse(const QVector<double> &diffuse) { diffuse_ = diffuse; }

    const QVector<double> &GetEmissive() const { return emissive_; }

    void SetEmissive(const QVector<double> &emissive) { emissive_ = emissive; }

    const QVector<double> &GetSpecular() const { return specular_; }

    void SetSpecular(const QVector<double> &specular) { specular_ = specular; }

    float GetOpacity() const { return opacity_; }

    void SetOpacity(float opacity) { opacity_ = opacity; }

    float GetShininess() const { return shininess_; }

    void SetShininess(float shininess) { shininess_ = shininess; }

    float GetReflectivity() { return reflectivity_; }

    void SetReflectivity(float reflectivity) { reflectivity_ = reflectivity; }

    void AddTexture(QSharedPointer<Texture> texture_data) { textures[texture_data->GetChannel()] = texture_data; }

    QSharedPointer<Texture> GetTexture(const QString &channel) { return textures[channel]; }

private:
    QString name_;
    Shading shading_;
    QVector<double> ambient_;
    QVector<double> diffuse_;
    QVector<double> specular_;
    QVector<double> emissive_;
    float opacity_ = 0.0f;
    float shininess_ = 0.0f;
    float reflectivity_ = 0.0f;
    QMap<QString, QSharedPointer<Texture>> textures;
};

#endif //QSPACE_MATERIAL_H
