# qtjson



## 简介
[qtjson] 是一个无依赖、head-only、开箱即用的自动的序列化和反序列化库。
完全基于qt自身的json相关对象实现。

本库是基于https://github.com/archibate/reflect-hpp 进行修改得到的。

## 使用

### 头文件添加到项目中
qtjson.hpp 以及 reflect.hpp

### 包含头文件


```
#include"qtjson.hpp"

```

### 使用作用域
```
using namespace qtjson; //也可以不声明
```

### 自定义类型
#### 成员注册
```

class OsInfo {
public:

    QString m_type = "Windows";
    QString m_platform = "win";
    QString m_release;
    QString m_hostname;
    QString m_arch;
    double  m_uptime;
	
	//成员注册
    REFLECT(m_type,m_platform,m_release,m_hostname,m_arch,m_uptime);
};

```

#### 序列化和反序列化

```
//自定义类型
void testCustom(){

    using namespace qtjson;
    //1.对象转换QJsonObject
    OsInfo * osInfo = new OsInfo();
    osInfo->m_platform ="平台";
    osInfo->m_arch = "x86";
    osInfo->m_hostname = "lenovo";
    osInfo->m_release = "win10212H";
    osInfo->m_uptime = 12.6;
    QJsonObject j1= objToJson(*osInfo);
    for (auto it = j1.begin(); it != j1.end(); ++it) {
        qDebug() << "Key:" << it.key() << ", Value:" << it.value().toString();
    }


    //2.QJsonObject 转换对象
    OsInfo o =  jsonToObj<OsInfo>(j1);
    qDebug()<< o.m_platform<<","
             <<o.m_arch<<","
             <<o.m_hostname<<","
             <<o.m_release<<","
             <<o.m_uptime;


    //对象转json字符串
    QString jsonStr = serialize(*osInfo);
    /*
        {
            "m_arch": "x86",
            "m_hostname": "lenovo",
            "m_platform": "平台",
            "m_release": "win10212H",
            "m_type": "Windows",
            "m_uptime": 12.6
        }
     */

    qDebug()<<jsonStr.toUtf8().data();

    //json字符串转对象
    OsInfo o2 =  deserialize<OsInfo>(jsonStr);
    qDebug()<< o2.m_platform<<","
             <<o2.m_arch<<","
             <<o2.m_hostname<<","
             <<o2.m_release<<","
             <<o2.m_uptime;





    delete osInfo;
}

```



## 自定义类型的扩展

对于非基本类型，例如std::map ,std::vector，需要特化处理。
### 添加特化类型
在qtjson.hpp末尾，qtjson作用域下，增加如下格式的模板类：
```

template <特化类型所使用的的泛型>
struct special_traits<特化类型> {
    static constexpr bool value = true;

    static QJsonValue objToJson(特化类型 const &object) {
        // 此函数中返回一个QJsonValue
    }

    static 特化类型 jsonToObj(QJsonValue const &root) {
       //此函数中，将QJsonValue 转换为  特化类型 即可
    }
};


```


### 示例
```

//1.std::vector
template <class T, class Alloc>
struct special_traits<std::vector<T, Alloc>> {
    static constexpr bool value = true;

    static QJsonValue objToJson(std::vector<T, Alloc> const &object) {
        QJsonArray root;
        for (auto const &elem: object) {

            qDebug()<< elem;
            qDebug()<<  qtjson::objToJson(elem);
            root.append(qtjson::objToJson(elem));
        }

        qDebug()<<root;
        return root;
    }

    static std::vector<T, Alloc> jsonToObj(QJsonValue const &root) {
        std::vector<T, Alloc> object;
        for (auto const &elem : root.toArray()) {
            object.push_back(qtjson::jsonToObj<T>(elem));
        }
        return object;
    }
};

//2.std::map
template <class K, class V, class Alloc>
struct special_traits<std::map<K, V, Alloc>> {
    static constexpr bool value = true;

    static QJsonValue objToJson(std::map<K, V, Alloc> const &object) {
        QJsonObject root;
        for (auto const &elem: object) {
            root[elem.first] = qtjson::objToJson(elem.second);
        }
        return root;
    }

    static std::map<K, V, Alloc> jsonToObj(QJsonValue const &root) {
        std::map<K, V, Alloc> object;
         QJsonObject  jsonObject = root.toObject();

        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
            const QString &key = it.key();
            const QJsonValue &value = it.value();
            object[key] = qtjson::jsonToObj<V>(root[key]);

        }
        return object;
    }
};


```







