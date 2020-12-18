#pragma once

#include "stdafx.h"
#include "GBNReceiver.h"
#include "Global.h"

GBNReceiver::GBNReceiver() :nextSeq(1)
{
	lastAckPkt.acknum = 0;
	lastAckPkt.seqnum = -1;
	memset(lastAckPkt.payload, '.', Configuration::PAYLOAD_SIZE);
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

GBNReceiver::~GBNReceiver()
{

}

void GBNReceiver::receive(const Packet& packet)
{
	if (pUtils->calculateCheckSum(packet) == packet.checksum && nextSeq == packet.seqnum)
	{
		pUtils->printPacket("接收方正确收到发送方的报文", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
		lastAckPkt.acknum = packet.seqnum;
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("接收方发送确认报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
		++nextSeq;
		return;
	}
	if (packet.seqnum != nextSeq)
		pUtils->printPacket("接收方未收到正确报文：报文序号错误", packet);
	else
		pUtils->printPacket("接收方未收到正确报文：检验和错误", packet);
	pUtils->printPacket("接受方重新发送上次的确认报文", lastAckPkt);
	pns->sendToNetworkLayer(SENDER, lastAckPkt);
}