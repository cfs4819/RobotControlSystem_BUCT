import json
import requests
import socket

url_base="http://api.heclouds.com"
dic_apikey={"user_yza":"uhbuJe0kMFu9gBkHUR9aKnXXYew=",
            "instructor_hxt":"zeEXLarZKmqLR3YAQi89xnKGdHU=",
            "master_key":"XQCo6coxRcs34otu4bKzeSbIZTA="}
dic_devices={"机械臂":"670138351",}
data_streams=["ds_feedback"]

devices_name="机械臂"
datastreams_count=0

user="instructor_hxt"
#动作1
start="*#10240#*\r\n"

def isNetOK(testserver):
    s=socket.socket()
    s.settimeout(3)
    try:
        status = s.connect_ex(testserver)
        if status == 0:
            s.close()
            return True
        else:
            return False
    except Exception as e:
        return False


def GetDatastream(devices_name,datastreams_count,access_user):
    url=url_base+"/devices/"+dic_devices[devices_name]+"/datastreams/"+data_streams[datastreams_count]
    data_dic=json.loads(requests.get(url,headers={"api-key":dic_apikey[access_user]},timeout=20).content)
    error_num=data_dic["errno"]
    if error_num :
        print("error:"+data_dic["error"])
        return
    final_data_dic=data_dic['data']
    return final_data_dic

def GetStatus(devices_name,access_user):
    url=url_base+"/devices/"+dic_devices[devices_name]
    data_dic=json.loads(requests.get(url,headers={"api-key":dic_apikey[access_user]},timeout=20).content)
    if data_dic["errno"] !=0:
        print("error num:",data_dic['errno'],"\r\nerror:",data_dic["error"])
    else:
        device_status= "online" if data_dic["data"]["online"]  else "offline"
    return device_status

def GetDeviceImformation(devices_name,access_user):
    url=url_base+"/devices/"+dic_devices[devices_name]
    data_dic=json.loads(requests.get(url,headers={"api-key":dic_apikey[access_user]},timeout=20).content)
    if data_dic["errno"] !=0:
        print("error num:",data_dic['errno'],"\r\nerror:",data_dic["error"])
    else:
        for i in data_dic["data"]:
            print(i,":",data_dic["data"][i])

'''***************信号解析***************
' * 信号格式1："*#10240#*\r\n{\"S1\":{\"a\":1500,\"t\":1000},\"S2\":{\"a\":1500,\"t\":1000},\"S3\":{\"a\":1500,\"t\":1000},\"S4\":{\"a\":1500,\"t\":1000},\"S5\":{\"a\":1500,\"t\":1000},\"S6\":{\"a\":1500,\"t\":1000}}"
' * 		即: "*#逐个控制模式#*\r\n*舵机编号#舵机角度#摆动时间"
' * 信号格式2："*#coordinate#*\r\n*10*20*120"
' * 		即: "*#坐标模式#*\r\n*X坐标*Y坐标*Z坐标"
' * 信号格式3："*#action_group#*\r\n*21#1000*22#1000"
' * 		即: "*#动作组模式#*\r\n*动作编号#运转时间"		//多个动作组则按顺序执行
' ***************************************'''
def SendCommand(commands,devices_name,access_user):
    url=url_base+"/cmds?"+"device_id="+dic_devices[devices_name]
    data_dic=json.loads(requests.post(url,data=commands,headers={"api-key":dic_apikey[access_user]},timeout=20).content)
    if data_dic["errno"] !=0:
        print("error num:",data_dic['errno'],"\r\nerror:",data_dic["error"])
    else:
        for i in data_dic["data"]:
            print(i,":",data_dic["data"][i])

#SendCommand("*#10240#*\r\n{\"S1\":{\"a\":1500,\"t\":1000},\"S2\":{\"a\":1500,\"t\":1000},\"S3\":{\"a\":1500,\"t\":1000},\"S4\":{\"a\":1500,\"t\":1000},\"S5\":{\"a\":1500,\"t\":1000},\"S6\":{\"a\":1500,\"t\":1000}}",\
#            "机械臂","master_key")


def sendActGroupOnnet(act_group):
    SendCommand(start + json.dumps(act_group),devices_name, user)

def packActGroup(act_group):
    return start + json.dumps(act_group)

def sendOtherCommandOnnet(startStr,act_group):
    SendCommand(startStr + json.dumps(act_group),devices_name, user)

def packOtherGroup(startStr,act_group):
    return startStr + json.dumps(act_group)

def openGrap():
    SendCommand("*#10240#*\r\n{\"S6\":{\"a\":500,\"t\":1000}}", devices_name, user)


def closeGrap():
    SendCommand("*#10240#*\r\n{\"S6\":{\"a\":1000,\"t\":1000}}", devices_name, user)


def initOneNET():
    try:
        whetherConnect=isNetOK(("api.heclouds.com",443))
    except:
        return False
    #print(whetherConnect)
    if whetherConnect:
        devstatus=GetStatus(devices_name, user)
        print("device status:\t" +devstatus)
        return devstatus
    else:
        return False


