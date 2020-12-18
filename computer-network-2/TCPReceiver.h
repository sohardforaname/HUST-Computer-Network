#pragma once

#include "RdtReceiver.h"

class TCPReceiver :public RdtReceiver
{
private:
	int nextSeq;
	Packet lastAckPkt;

public:
	void receive(const Packet& packet);

public:
	TCPReceiver();
	~TCPReceiver();
	
};