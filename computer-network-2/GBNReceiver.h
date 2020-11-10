#pragma once

#include "RdtReceiver.h"

class GBNReceiver : public RdtReceiver
{
private:
	int nextSeq;
	Packet lastAckPkt;
	
public:
	void receive(const Packet& packet);

public:
	GBNReceiver();
	~GBNReceiver();

};