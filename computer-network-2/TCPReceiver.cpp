#pragma once

#include "stdafx.h"
#include "Global.h"
#include "TCPReceiver.h"

TCPReceiver::TCPReceiver() :nextSeq(1)
{
	lastAckPkt.acknum = 0;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

TCPReceiver::~TCPReceiver()
{

}

void TCPReceiver::receive(const Packet& packet)
{
	if (pUtils->calculateCheckSum(packet) == packet.checksum && nextSeq == packet.seqnum)
	{
		pUtils->printPacket("接收方正确收到发送方的报文", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
		lastAckPkt.acknum = ++nextSeq;
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("接收方发送ACK报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
		return;
	}
	if (packet.seqnum != nextSeq)
	{
		printf("应得的报文序号：%d\n", nextSeq);
		pUtils->printPacket("接收方未正确收到报文：报文序列号错误", packet);
	}
	else
		pUtils->printPacket("接收方未正确收到报文：检验和错误", packet);
	pUtils->printPacket("接收方发送冗余ACK", lastAckPkt);
	pns->sendToNetworkLayer(SENDER, lastAckPkt);
}