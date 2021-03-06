#pragma once

#include "stdafx.h"
#include "GBNSender.h"
#include "Global.h"
#include <cassert>


GBNSender::GBNSender() :base(1), nextSeq(1), waitingState(false), curPkt(0)
{

}

GBNSender::~GBNSender()
{

}

bool GBNSender::getWaitingState()
{
	return waitingState;
}

bool GBNSender::send(const Message& message)
{
	if (nextSeq < base + windowSize)
	{
		waitingState = false;
		window[curPkt].acknum = -1;
		window[curPkt].seqnum = nextSeq;
		memcpy(window[curPkt].payload, message.data, sizeof(message.data));
		window[curPkt].checksum = pUtils->calculateCheckSum(window[curPkt]);
		pUtils->printPacket("发送方发送报文", window[curPkt]);
		if (base == nextSeq)
			pns->startTimer(SENDER, Configuration::TIME_OUT, window[curPkt].seqnum);
		pns->sendToNetworkLayer(RECEIVER, window[curPkt]);
		++curPkt;
		++nextSeq;
		if (curPkt > windowSize)
			waitingState = true;
		return true;
	}
	assert(waitingState);
	waitingState = true;
	return false;
}

void GBNSender::receive(const Packet& ackpkt)
{
	if (curPkt > 0 && pUtils->calculateCheckSum(ackpkt) == ackpkt.checksum && ackpkt.acknum >= base)
	{
		int pktSum = ackpkt.acknum - base + 1;
		base = ackpkt.acknum + 1;
		pns->stopTimer(SENDER, window[0].seqnum);
		if (base != nextSeq)
			pns->startTimer(SENDER, Configuration::TIME_OUT, window[pktSum].seqnum);
		showWindow(0);
		for (int i = 0; i < curPkt - pktSum; ++i)
			window[i] = window[i + pktSum];
		showWindow(1);
		printf("发送方当前base: %d\n", base);
		curPkt -= pktSum;
	}
}

void GBNSender::timeoutHandler(int seqNum)
{
	pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", window[0]);
	pns->stopTimer(SENDER, window[0].seqnum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, window[0].seqnum);
	for (int i = 0; i < curPkt; ++i)
		pns->sendToNetworkLayer(RECEIVER, window[i]);
}

void GBNSender::showWindow(int op)
{
	printf(!op ? "发送方移动前：\n" : "发送方移动后：\n");
	printf("[\n");
	for (int i = 0; i < windowSize / 2; ++i)
		pUtils->printPacket("", window[i]);
	printf("]\n");
}