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
		pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
		lastAckPkt.acknum = packet.seqnum;
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
		++nextSeq;
		return;
	}
	if (packet.seqnum != nextSeq)
		pUtils->printPacket("���շ�δ�յ���ȷ���ģ�������Ŵ���", packet);
	else
		pUtils->printPacket("���շ�δ�յ���ȷ���ģ�����ʹ���", packet);
	pUtils->printPacket("���ܷ����·����ϴε�ȷ�ϱ���", lastAckPkt);
	pns->sendToNetworkLayer(SENDER, lastAckPkt);
}