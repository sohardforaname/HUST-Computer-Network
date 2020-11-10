#pragma once

#include "stdafx.h"
#include "Global.h"
#include "TCPSender.h"

TCPSender::TCPSender() :base(1), nextSeq(1), curPkt(0), waitingState(false), count(0)
{
	memset(tag, 0, sizeof(tag));
}

TCPSender::~TCPSender()
{

}

bool TCPSender::getWaitingState()
{
	return waitingState;
}

bool TCPSender::send(const Message& message)
{
	if (nextSeq < base + windowSize / 2);
	{
		waitingState = false;
		window[curPkt].acknum = -1;
		window[curPkt].seqnum = nextSeq;
		memcpy(window[curPkt].payload, message.data, sizeof(message.data));
		window[curPkt].checksum = pUtils->calculateCheckSum(window[curPkt]);
		pUtils->printPacket("发送方发送报文", window[curPkt]);
		if (base == nextSeq)//如果是base的报文，则启动定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, window[curPkt].seqnum);
		pns->sendToNetworkLayer(RECEIVER, window[curPkt]);//发送报文至网络层
		++curPkt;
		++nextSeq;
		return true;
	}
	waitingState = true;
	return false;
}

void TCPSender::receive(const Packet& packet)
{
	if (curPkt && packet.checksum == pUtils->calculateCheckSum(packet))
	{
		pUtils->printPacket("发送方接收到ACK报文", packet);
		printf("发送方当前base是：%d\n", base);
		if (packet.acknum == window[0].seqnum)
		{
			++count;
			if (count == 3)
			{
				pns->stopTimer(SENDER, window[0].seqnum);
				pns->sendToNetworkLayer(RECEIVER, window[0]);
				printf("发送方冗余ACK %d *3", packet.acknum);
				pUtils->printPacket("，快速重传当前窗口第一个报文", window[0]);
				showWindow();
				pns->startTimer(SENDER, Configuration::TIME_OUT, window[0].seqnum);
				count = 0;
				return;
			}
		}
		else
			count = 1;
		if (count != 1)
			return;
		int maxAck = packet.acknum - base;
		base = packet.acknum;
		pns->stopTimer(SENDER, window[0].seqnum);
		pUtils->printPacket("发送方接收到ACK报文", packet);
		if (base != nextSeq)
			pns->startTimer(SENDER, Configuration::TIME_OUT, window[maxAck].seqnum);
		showWindow();
		for (int i = 0; i <= curPkt - maxAck; ++i)
			window[i] = window[i + maxAck];
		showWindow();
		curPkt -= maxAck;
	}
}

void TCPSender::timeoutHandler(int seqNum)
{
	pUtils->printPacket("发送方定时器时间到，重发报文", window[0]);
	pns->stopTimer(SENDER, window[0].seqnum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, window[0].seqnum);
	pns->sendToNetworkLayer(RECEIVER, window[0]);
}

void TCPSender::showWindow()
{
	puts("发送方：");
	printf("[\n");
	for (int i = 0; i < windowSize / 2; ++i)
		pUtils->printPacket("", window[i]);
	printf("]\n");
}