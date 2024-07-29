
#include <QApplication>
#include<QDebug>
#include"qtjson.hpp"
#include<QJsonArray>
class OsInfo {
public:

    QString m_type = "Windows";//系统类型，Windows 或 Linux
    QString m_platform = "win";
    QString m_release;//系统版本
    QString m_hostname;//机器名
    QString m_arch;//cpu架构
    double  m_uptime;//开机时间

    QJsonObject toJson();
    REFLECT(m_type,m_platform,m_release,m_hostname,m_arch,m_uptime);
};

//自定义类型
void testCustom(){

    using namespace qtjson;
    //对象转换QJsonObject
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


    //QJsonObject 转换对象
    OsInfo o =  jsonToObj<OsInfo>(j1);
    qDebug()<< o.m_platform<<","
             <<o.m_arch<<","
             <<o.m_hostname<<","
             <<o.m_release<<","
             <<o.m_uptime;


    //对象转json字符串
    QString jsonStr =jsonToStr( objToJson(*osInfo));
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
    OsInfo o2 =   jsonToObj<OsInfo>(strToJson(jsonStr));
    qDebug()<< o2.m_platform<<","
             <<o2.m_arch<<","
             <<o2.m_hostname<<","
             <<o2.m_release<<","
             <<o2.m_uptime;



    // 创建 QJsonArray
    QJsonArray jsonArray;
    jsonArray.append(1);
    jsonArray.append(2);
    jsonArray.append(3);
    QJsonObject jsonObject;
    jsonObject["numbers"] = jsonArray;



    qDebug()<< jsonObject["numbers"] ;
}

//测试 std::vector
void testVector(){
    using namespace qtjson;


    //序列化
    std::vector<int> v= {
        1,2,3,4,5,6,8
    };
    QString jsonStr = jsonToStr( objToJson(v));
    qDebug()<<jsonStr.toUtf8().data();


    //反序列化
    std::vector<int> v2=   jsonToObj< std::vector<int>>( strToJson(jsonStr));
    qDebug()<<"v2.size="<< v2.size();
    for(auto & elem : v2){
        qDebug()<<"反序列化:"<<elem;
    }
    qDebug()<<"反序列化结束";
}


void testMap(){
    using namespace qtjson;

    std::map<QString,int> m={

        {"zhangsan",20},
        {"lisi",15}
    };
    QString jsonStr = jsonToStr( objToJson(m));
    qDebug()<<jsonStr.toUtf8().data();


    std::map<QString,int> m2=   jsonToObj< std::map<QString,int>>( strToJson(jsonStr));
    qDebug()<<m2.size();
    for (auto p : m2) {
       qDebug()<< "key=" << p.first << ",value=" << p.second;
    }



}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // testCustom();
    // testVector();
    testMap();


    return a.exec();
}
