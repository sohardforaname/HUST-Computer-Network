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
		pUtils->printPacket("���ͷ����ͱ���", window[curPkt]);
		if (base == nextSeq)//�����base�ı��ģ���������ʱ��
			pns->startTimer(SENDER, Configuration::TIME_OUT, window[curPkt].seqnum);
		pns->sendToNetworkLayer(RECEIVER, window[curPkt]);//���ͱ����������
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
		pUtils->printPacket("���ͷ����յ�ACK����", packet);
		printf("���ͷ���ǰbase�ǣ�%d\n", base);
		if (packet.acknum == window[0].seqnum)
		{
			++count;
			if (count == 3)
			{
				pns->stopTimer(SENDER, window[0].seqnum);
				pns->sendToNetworkLayer(RECEIVER, window[0]);
				printf("���ͷ�����ACK %d *3", packet.acknum);
				pUtils->printPacket("�������ش���ǰ���ڵ�һ������", window[0]);
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
		pUtils->printPacket("���ͷ����յ�ACK����", packet);
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
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط�����", window[0]);
	pns->stopTimer(SENDER, window[0].seqnum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, window[0].seqnum);
	pns->sendToNetworkLayer(RECEIVER, window[0]);
}

void TCPSender::showWindow()
{
	puts("���ͷ���");
	printf("[\n");
	for (int i = 0; i < windowSize / 2; ++i)
		pUtils->printPacket("", window[i]);
	printf("]\n");
}