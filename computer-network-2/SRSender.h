#pragma once

#include "RdtSender.h"

class SRSender :public RdtSender
{
private:
	Packet window[windowSize << 1];
	int tag[windowSize << 1];
	int base, nextSeq, curPkt;
	bool waitingState;

public:
	bool send(const Message& message);
	void receive(const Packet& ackpkt);
	void timeoutHandler(int seqNum);
	bool getWaitingState();
	void ShowWindow();

public:
	SRSender();
	virtual ~SRSender();

};
