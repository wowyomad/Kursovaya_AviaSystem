#pragma once

class BaseFile
{
public:
	virtual void CreateFIle() = 0;
	virtual bool OpenFile() = 0;
	virtual void ClearFile() = 0;
	virtual void CloseFile() = 0;
	virtual void ReadFileToVector() = 0;
	virtual void WriteVectorToFile() = 0;
	virtual int getFileInfo(size_t& numOfObjects, size_t& fileSize) = 0;
};