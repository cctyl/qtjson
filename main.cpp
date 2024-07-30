
#include <QApplication>
#include<QDebug>
#include"qtjson.hpp"
#include<QJsonArray>
#include<map>
#include<vector>
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
class CpuInfo{

public:

    double cpuUsage;
    unsigned short cpuCount;
    QString cpuModel;

    REFLECT(cpuUsage,cpuCount,cpuModel)
};

class MemInfo{


public:
    double totalMemMb;
    double usedMemMb;
    double freeMemMb;
    double usedMemPercentage;
    double freeMemPercentage;

    REFLECT(totalMemMb,usedMemMb,freeMemMb,usedMemPercentage,freeMemPercentage)
};
class DriverInfo{

public:
    double totalGb;
    double usedGb;
    double   freeGb ;
    double usedPercentage;
    double freePercentage;
    REFLECT(totalGb,usedGb,usedPercentage,freePercentage)
};

/**
 * 网速相关信息
 * @brief The NetstatInfo class
 */
class NetstatInfo{
public:
    double inputMb;
    double outputMb;
    REFLECT(inputMb,outputMb)
};

/**
 * 网卡信息
 *
 * @brief The NetInterfaceInfo class
 */
class NetInterfaceInfo{
public:

    QString address;
    QString netmask;
    QString family;
    QString mac;
    bool internal;
    REFLECT(address,netmask,family,mac,internal)
};





class DevInfo
{
public:
    CpuInfo cpuInfo;
    MemInfo memInfo;
    DriverInfo driveInfo;
    std::map<QString,NetstatInfo> netstatInfo;
    std::map<QString,QJsonArray> netInterface;
    OsInfo osInfo;
    short openedCount = 0;
    QJsonObject ipInfo;

    REFLECT(cpuInfo,memInfo,driveInfo,netstatInfo,netInterface,osInfo,openedCount,ipInfo)
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

//测试 std::vector
void testVector(){
    using namespace qtjson;


    //序列化
    std::vector<int> v= {
        1,2,3,4,5,6,8
    };
    QString jsonStr = serialize(v);
    qDebug()<<jsonStr.toUtf8().data();


    //反序列化
    std::vector<int> v2=   deserialize<std::vector<int>>( jsonStr);
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
    QString jsonStr = serialize(m);
    qDebug()<<jsonStr.toUtf8().data();


    std::map<QString,int> m2=deserialize< std::map<QString,int>>( jsonStr);
    qDebug()<<m2.size();
    for (auto p : m2) {
        qDebug()<< "key=" << p.first << ",value=" << p.second;
    }



}


void testComplex(){

    QString jsonStr = R"({
    "cpuInfo": {
      "cpuUsage": 0,
      "cpuCount": 2,
      "cpuModel": "Intel(R) Celeron(R) CPU  J1800  @ 2.41GHz"
    },
    "memInfo": {
      "totalMemMb": 3831.13,
      "usedMemMb": 514.58,
      "freeMemMb": 3316.55,
      "usedMemPercentage": 13.43,
      "freeMemPercentage": 86.57
    },
    "driveInfo": {
      "totalGb": "12.9",
      "usedGb": "8.3",
      "freeGb": "4.6",
      "usedPercentage": "64.1",
      "freePercentage": "35.9"
    },
    "netstatInfo": {
      "total": {
        "inputMb": 0,
        "outputMb": 0
      },
      "enp1s0": {
        "inputMb": 0,
        "outputMb": 0
      },
      "wgp": {
        "inputMb": 0,
        "outputMb": 0
      },
      "wgc": {
        "inputMb": 0,
        "outputMb": 0
      }
    },
    "netInterface": {
      "lo": [
        {
          "address": "127.0.0.1",
          "netmask": "255.0.0.0",
          "family": "IPv4",
          "mac": "00:00:00:00:00:00",
          "internal": true,
          "cidr": "127.0.0.1/8"
        },
        {
          "address": "::1",
          "netmask": "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",
          "family": "IPv6",
          "mac": "00:00:00:00:00:00",
          "internal": true,
          "cidr": "::1/128",
          "scopeid": 0
        }
      ],
      "enp1s0": [
        {
          "address": "192.168.0.9",
          "netmask": "255.255.255.0",
          "family": "IPv4",
          "mac": "54:f6:c5:fb:0f:db",
          "internal": false,
          "cidr": "127.0.0.10/24"
        },
        {
          "address": "fe80::56f6:c6ff:fefb:fdb",
          "netmask": "ffff:ffff:ffff:ffff::",
          "family": "IPv6",
          "mac": "54:f6:c5:fb:0f:db",
          "internal": false,
          "cidr": "fe80::56f6:c5ff:fefb:fdb/64",
          "scopeid": 2
        }
      ],
      "wgp": [
        {
          "address": "127.0.0.10",
          "netmask": "255.255.255.255",
          "family": "IPv4",
          "mac": "00:00:00:00:00:00",
          "internal": false,
          "cidr": "127.0.0.10/32"
        }
      ],
      "wgc": [
        {
          "address": "127.0.0.10",
          "netmask": "255.255.255.255",
          "family": "IPv4",
          "mac": "00:00:00:00:00:00",
          "internal": false,
          "cidr": "127.0.0.10/32"
        }
      ]
    },
    "osInfo": {
      "type": "Linux",
      "platform": "linux",
      "release": "4.15.0-213-generic",
      "ip": "127.0.0.1",
      "hostname": "linux",
      "arch": "x64",
      "uptime": 345918.83
    },
    "openedCount": 0,
    "ipInfo": {
      "status": "success",
      "country": "中国",
      "countryCode": "CN",
      "region": "GD",
      "regionName": "江苏",
      "city": "吴川",
      "zip": "",
      "lat": 23.4508,
      "lon": 120.7633,
      "timezone": "Asia/Shanghai",
      "isp": "Chinanet",
      "org": "Chinanet GD",
      "as": "AS4134 CHINANET-BACKBONE",
      "query": "127.0.0.1"
    }
  })";

    QJsonObject obj = qtjson::strToJson(jsonStr).toObject();
    // 获取 "driveInfo" 子对象
    QJsonValue driveInfoValue = obj.value("driveInfo");
    if (!driveInfoValue.isNull() && driveInfoValue.isObject()) {
        QJsonObject driveInfo = driveInfoValue.toObject();

        // 从 "driveInfo" 子对象中获取 "usedPercentage"
        QJsonValue usedPercentageValue = driveInfo.value("usedPercentage");
        qDebug() <<"usedPercentage type ="<<usedPercentageValue.type();
        if (!usedPercentageValue.isNull() && usedPercentageValue.isDouble()) {
            double usedPercentage = usedPercentageValue.toDouble();
            qDebug() << "usedPercentage:" << usedPercentage;
        } else {
            qDebug() << "usedPercentage is not a valid number";
        }
    } else {
        qDebug() << "driveInfo is not a valid object";
    }




    // DevInfo d =  qtjson::deserialize<DevInfo>(jsonStr);
    // qDebug()<<"";
    // qDebug()<<"=====================";
    // qDebug()<<d.cpuInfo.cpuModel;

    // for(auto & pair :d.netInterface){

    //     qDebug()<<"map key="<<pair.first;
    //     qDebug()<<"map value="<<pair.second;

    //     std::vector<NetInterfaceInfo> v =  qtjson::jsonArrayToVector<NetInterfaceInfo>(pair.second);
    //     qDebug()<<"v.size= "<<v.size();
    //     for(NetInterfaceInfo &n :v){
    //         qDebug()<<"address="<<n.address;

    //     }
    // }

    // qDebug()<<"driverInfo.usedPercentage="<<d.driveInfo.usedPercentage;


    // qDebug()<<"";
    // qDebug()<<"=========反序列化============";
    // qDebug()<< qtjson::serialize(d).toUtf8().data();


}


void testComplex2(){

    QString jsonStr = R"({
  "enp1s0": [
    {
      "address": "192.168.0.9",
      "cidr": "127.0.0.10/24",
      "family": "IPv4",
      "internal": false,
      "mac": "54:f6:c5:fb:0f:db",
      "netmask": "255.255.255.0"
    },
    {
      "address": "fe80::56f6:c6ff:fefb:fdb",
      "cidr": "fe80::56f6:c5ff:fefb:fdb/64",
      "family": "IPv6",
      "internal": false,
      "mac": "54:f6:c5:fb:0f:db",
      "netmask": "ffff:ffff:ffff:ffff::",
      "scopeid": 2
    }
  ],
  "lo": [
    {
      "address": "127.0.0.1",
      "cidr": "127.0.0.1/8",
      "family": "IPv4",
      "internal": true,
      "mac": "00:00:00:00:00:00",
      "netmask": "255.0.0.0"
    },
    {
      "address": "::1",
      "cidr": "::1/128",
      "family": "IPv6",
      "internal": true,
      "mac": "00:00:00:00:00:00",
      "netmask": "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",
      "scopeid": 0
    }
  ],
  "wgc": [
    {
      "address": "127.0.0.10",
      "cidr": "127.0.0.10/32",
      "family": "IPv4",
      "internal": false,
      "mac": "00:00:00:00:00:00",
      "netmask": "255.255.255.255"
    }
  ],
  "wgp": [
    {
      "address": "127.0.0.10",
      "cidr": "127.0.0.10/32",
      "family": "IPv4",
      "internal": false,
      "mac": "00:00:00:00:00:00",
      "netmask": "255.255.255.255"
    }
  ]
}
)";

    std::map<QString,QJsonArray> netInterface = qtjson::deserialize<std::map<QString,QJsonArray>>(jsonStr);

    for(auto & pair : netInterface){

        qDebug()<<"map key="<<pair.first;
        qDebug()<<"map value="<<pair.second;
        //传入QJsonValue 返回 QJsonArray
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // testCustom();
    // testVector();
    //testMap();

    testComplex();
    //testComplex2();
    return a.exec();
}
