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
		pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
		lastAckPkt.acknum = ++nextSeq;
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("���շ�����ACK����", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
		return;
	}
	if (packet.seqnum != nextSeq)
	{
		printf("Ӧ�õı�����ţ�%d\n", nextSeq);
		pUtils->printPacket("���շ�δ��ȷ�յ����ģ��������кŴ���", packet);
	}
	else
		pUtils->printPacket("���շ�δ��ȷ�յ����ģ�����ʹ���", packet);
	pUtils->printPacket("���շ���������ACK", lastAckPkt);
	pns->sendToNetworkLayer(SENDER, lastAckPkt);
}