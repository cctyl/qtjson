
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


void test(){


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
    qDebug()<<jsonStr;

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
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    test();

    return a.exec();
}
