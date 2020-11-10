#pragma once

#include "RdtSender.h"

class GBNSender :public RdtSender
{
private:
	Packet window[windowSize + 5];
	int base, nextSeq, curPkt;
	bool waitingState;
	
public:
	bool send(const Message& message);
	void receive(const Packet& ackpkt);
	void timeoutHandler(int seqNum);
	bool getWaitingState();
	void showWindow();

public:
	GBNSender();
	virtual ~GBNSender();

};