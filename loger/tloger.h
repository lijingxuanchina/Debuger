#ifndef __TLOGER_H_
#define __TLOGER_H_

class TLoger
{
public:
	TLoger();
	~TLoger();
	void log(const CString& logInfo);

private:
	void makeSureLogFileExist();

private:
	CString m_tlogPath;
};

#endif // __TLOGER_H_