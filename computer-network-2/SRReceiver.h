#pragma once

#include "RdtReceiver.h"

class SRReceiver :public RdtReceiver
{
private:
	int base, totPkt;
	Packet window[windowSize << 1], lastAckPkt;
	int tag[windowSize << 1];

public:
	void receive(const Packet& packet);
	void showWindow(int op);

public:
	SRReceiver();
	~SRReceiver();
};