#include "Material.h"

QMap<QString, QString> Material::texturePaths() {
    QMap<QString, QString> texts;
    texts["diffuse"] = m_diffusePath;
    return texts;
};
