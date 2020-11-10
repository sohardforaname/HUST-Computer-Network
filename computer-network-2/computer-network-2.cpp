// computer-network-2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include "Global.h"
#include "GBNSender.h"
#include "GBNReceiver.h"
#include "TCPSender.h"
#include "TCPReceiver.h"
#include "SRSender.h"
#include "SRReceiver.h"
#include <iostream>

int main()
{
	RdtSender* ps = new TCPSender();
	RdtReceiver* pr = new TCPReceiver();
	//pns->setRunMode(0);  //VERBOS模式
	pns->setRunMode(1);  //安静模式
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("D:\\code\\vs\\C++_code\\computer-network\\Windows_VS2017\\input.txt");
	pns->setOutputFile("D:\\code\\vs\\C++_code\\computer-network\\Windows_VS2017\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete

	return 0;
}


