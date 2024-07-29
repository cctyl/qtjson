#ifndef QTJSON_H
#define QTJSON_H

#include"reflect.hpp"
#include<QString>
#include<QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include<QJsonArray>
#include<vector>
#include<QDebug>


// ============================================================================
template <class T>
struct special_traits {
    static constexpr bool value = false;
};

template <class T, std::enable_if_t<!reflect::has_member<T>() && !special_traits<T>::value, int> = 0>
QJsonValue objToJson(T const &object) {
    return object;
}

template <class T, std::enable_if_t<!reflect::has_member<T>() && special_traits<T>::value, int> = 0>
QJsonObject objToJson(T const &object) {
    return special_traits<T>::objToJson(object);
}

template <class T, std::enable_if_t<reflect::has_member<T>(), int> = 0>
QJsonObject objToJson(T const &object) {
    QJsonObject root;
    reflect::foreach_member(object, [&](const char *key, auto &value) {
        root[key] = objToJson(value);
    });
    return root;
}



template <class T, std::enable_if_t<!reflect::has_member<T>() && !special_traits<T>::value, int> = 0>
T jsonToObj(QJsonValue const &root) {

    static_assert(std::is_same<T, T>::value, "Type not supported");
    return T(); // 返回默认构造的 T 对象
}
// 特化 QString
template <>
QString jsonToObj<QString>(QJsonValue const &root) {
    if (root.isString()) {
        return root.toString();
    }
    return QString(); // 返回空字符串
}
// 特化 QArray
template <>
QJsonArray jsonToObj<QJsonArray>(QJsonValue const &root) {
    if (root.isArray()) {
        root.toArray();
    }
    return QJsonArray(); // 返回空字符串
}


// 特化 int
template <>
int jsonToObj<int>(QJsonValue const &root) {
    if (root.isDouble()) {
        return static_cast<int>(root.toDouble());
    }
    return 0; // 返回零
}
// 特化 bool
template <>
bool jsonToObj<bool>(QJsonValue const &root) {
    if (root.isBool()) {
        return root.toBool();
    }
    return false; // 返回 false
}

// 特化 double
template <>
double jsonToObj<double>(QJsonValue const &root) {
    if (root.isDouble()) {
        return root.toDouble();
    }
    return 0.0; // 返回零
}
// 特化 QJsonObject
template <>
QJsonObject jsonToObj<QJsonObject>(QJsonValue const &root) {
    if (root.isObject()) {
        return root.toObject();
    }
    return QJsonObject(); // 返回空对象
}




template <class T, std::enable_if_t<!reflect::has_member<T>() && special_traits<T>::value, int> = 0>
T jsonToObj(QJsonValue const &root) {
    return special_traits<T>::jsonToObj(root);
}

template <class T, std::enable_if_t<reflect::has_member<T>(), int> = 0>
T jsonToObj(QJsonObject const &root) {
    T object;
    reflect::foreach_member(object, [&](const char *key, auto &value) {
        value = jsonToObj<std::decay_t<decltype(value)>>(root[key]);
    });
    return object;
}


// =====================================================================
/**
 * QJsonObject -> QString
 * @brief jsonToStr
 * @param obj
 * @return
 */
inline QString jsonToStr(QJsonObject obj) {
    // 转换为 QJsonDocument
    QJsonDocument doc(obj);

    // 将 QJsonDocument 转换为 QByteArray
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented); // 使用 Indented 格式化输出

    // 将 QByteArray 转换为 QString
    return QString::fromUtf8(jsonData);
}
/**
 * QString -> QJsonObject
 * @brief strToJson
 * @param jsonString
 * @return
 */
inline QJsonObject strToJson(QString const &jsonString) {
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    // 检查是否解析成功
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error at offset" << parseError.offset << ":" << parseError.errorString();
        return QJsonObject();
    }

    // 获取 QJsonObject
    QJsonObject jsonObject = jsonDoc.object();
    return jsonObject;
}


// ==============================================================
//自定义特化扩展


// template <class T, class Alloc>
// struct special_traits<std::vector<T, Alloc>> {
//     static constexpr bool value = true;

//     static QJsonArray objToJson(std::vector<T, Alloc> const &object) {
//         QJsonObject root;
//         for (auto const &elem: object) {
//             root.append(reflect_json::objToJson(elem));
//         }
//         return root;
//     }

//     static std::vector<T, Alloc> jsonToObj(Json::Value const &root) {
//         std::vector<T, Alloc> object;
//         for (auto const &elem: root) {
//             object.push_back(reflect_json::jsonToObj<T>(elem));
//         }
//         return object;
//     }
// };

// template <class K, class V, class Alloc>
// struct special_traits<std::map<K, V, Alloc>> {
//     static constexpr bool value = true;

//     static Json::Value objToJson(std::map<K, V, Alloc> const &object) {
//         Json::Value root;
//         for (auto const &elem: object) {
//             root[elem.first] = reflect_json::objToJson(elem.second);
//         }
//         return root;
//     }

//     static std::map<K, V, Alloc> jsonToObj(Json::Value const &root) {
//         std::map<K, V, Alloc> object;
//         for (auto const &key: root.getMemberNames()) {
//             object[key] = reflect_json::jsonToObj<V>(root[key]);
//         }
//         return object;
//     }
// };




#endif // QTJSON_H
