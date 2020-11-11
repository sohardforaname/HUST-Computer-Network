#pragma once

#include "stdafx.h"
#include "Global.h"
#include "SRReceiver.h"

SRReceiver::SRReceiver() :totPkt(0), base(0)
{
	lastAckPkt.acknum = 0;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
	memset(tag, 0, sizeof(tag));
}

SRReceiver::~SRReceiver()
{
	
}

void SRReceiver::receive(const Packet& packet)
{
	if (pUtils->calculateCheckSum(packet) == packet.checksum)
	{
		int pos = -1;
		for (int i = 0; i < windowSize / 2; ++i)
			if (packet.seqnum == (base + i) % windowSize)
			{
				pos = i;
				break;
			}
		if (pos != -1)
		{
			tag[pos] = 1;
			window[pos] = packet;
			lastAckPkt.acknum = window[0].seqnum;
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
			if (tag[0])
			{
				pUtils->printPacket("接收方收到base的报文", window[0]);
				int maxAck = 0;
				for (int i = 0; i < windowSize / 2 && tag[i]; ++i)
					++maxAck;
				Message msg;
				for (int i = 0; i < maxAck; ++i)
				{
					memcpy(msg.data, window[i].payload, sizeof(window[i].payload));
					pns->delivertoAppLayer(RECEIVER, msg);
				}
				showWindow(0);
				for (int i = 0; i <= windowSize / 2 - maxAck; ++i)
				{
					window[i] = window[i + maxAck];
					tag[i] = tag[i + maxAck];
				}
				showWindow(1);
				for (int i = windowSize / 2 - maxAck + 1; i < windowSize / 2; ++i)
					tag[i] = 0;
				base = (base + maxAck) % windowSize;
				printf("接收方当前base: %d\n", base);
				return;
			}
			pUtils->printPacket("接收方非base报文", window[pos]);
			return;
		}
		pUtils->printPacket("接收方收到发送方的非窗口内的报文", packet);
		lastAckPkt.acknum = packet.seqnum;
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
		return;
	}
	pUtils->printPacket("接收方未正确收到报文:检验和错误", packet);
}

void SRReceiver::showWindow(int op)
{
	puts(!op ? "接收方移动前：" : "接收方移动后");
	printf("[\n");
	for (int i = 0; i < windowSize / 2; ++i)
		printf("tag: %d packet ", tag[i]), pUtils->printPacket("", window[i]);
	printf("]\n");
}