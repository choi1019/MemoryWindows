#pragma once

#include <time.h>
class PTimer {
private:
	clock_t m_tStart, m_tFinish;
	double  m_dDuration;
public:
	PTimer() {}
	~PTimer() {}

	void Intitlaize() {}
	void Finalize() {}

	void Start() {
		this->m_tStart = clock();
	}
	double Stop() {
		this->m_tFinish = clock();
		return (double)(m_tFinish - m_tStart);
	}
	unsigned GetClocksPerSecond() {
		return CLOCKS_PER_SEC;
	}
};