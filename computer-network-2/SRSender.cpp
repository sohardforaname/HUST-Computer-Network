#pragma once

#include "stdafx.h"
#include "Global.h"
#include "SRSender.h"

SRSender::SRSender() :base(0), nextSeq(0), waitingState(false), curPkt(0)
{
	memset(tag, 0, sizeof(tag));
}

SRSender::~SRSender()
{

}

bool SRSender::getWaitingState()
{
	return waitingState;
}

bool SRSender::send(const Message& message)
{
	if (curPkt < windowSize / 2)
	{
		if (!curPkt)
			memset(tag, 0, sizeof(tag));
		waitingState = false;
		window[curPkt].acknum = -1;
		window[curPkt].seqnum = nextSeq;
		memcpy(window[curPkt].payload, message.data, sizeof(message.data));
		window[curPkt].checksum = pUtils->calculateCheckSum(window[curPkt]);
		pUtils->printPacket("发送方发送报文", window[curPkt]);
		pns->startTimer(SENDER, Configuration::TIME_OUT, window[curPkt].seqnum);
		pns->sendToNetworkLayer(RECEIVER, window[curPkt]);
		++curPkt;
		nextSeq = (nextSeq + 1) % windowSize;
		if (curPkt == windowSize / 2)
			waitingState = true;
		return true;
	}
	waitingState = true;
	return false;
}

void SRSender::receive(const Packet& ackpkt)
{
	if (pUtils->calculateCheckSum(ackpkt) == ackpkt.checksum)
	{
		int pos = -1;
		for (int i = 0; i < curPkt; ++i)
			if (ackpkt.acknum == window[i].seqnum)
			{
				pos = i;
				break;
			}
		if (pos != -1)
		{
			tag[pos] = 1;
			pns->stopTimer(SENDER, window[pos].seqnum);
			if (tag[0])
			{
				pUtils->printPacket("发送方收到base的ACK", window[0]);
				int maxAck = 0;
				for (int i = 0; i < windowSize / 2 && tag[i]; ++i)
					++maxAck;
				ShowWindow(0);
				for (int i = 0; i <= curPkt - maxAck; ++i)
				{
					window[i] = window[i + maxAck];
					tag[i] = tag[i + maxAck];
				}
				ShowWindow(1);
				for (int i = curPkt - maxAck; i < windowSize / 2; ++i)
					tag[i] = 0;
				curPkt -= maxAck;
				base = (base + maxAck) % windowSize;
				waitingState = false;
				printf("发送方当前base: %d\n", base);
				return;
			}
			pUtils->printPacket("发送方收到窗口内非base的ACK", ackpkt);
			return;
		}
		pns->stopTimer(SENDER, ackpkt.acknum);
		pUtils->printPacket("发送方收到窗口之前报文的ACK", ackpkt);
		return;
	}
	pUtils->printPacket("发送方收到ACK包校验和出错", ackpkt);
}

void SRSender::timeoutHandler(int seqNum)
{
	if (base == nextSeq)
		return;
	int i = 0;
	while (window[i].seqnum != seqNum)
		i++;
	pns->stopTimer(SENDER, seqNum);
	pUtils->printPacket("发送方定时器超时，重新发送报文段", window[i]);
	pns->sendToNetworkLayer(RECEIVER, window[i]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
}

void SRSender::ShowWindow(int op)
{
	printf(!op ? "发送方移动前：\n" : "发送方移动后：\n");
	printf("[\n");
	for (int i = 0; i < windowSize / 2; ++i)
		printf("tag: %d packet ", tag[i]), pUtils->printPacket("", window[i]);
	printf("]\n");
}