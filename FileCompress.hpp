#pragma once

#include"HuffmanTree.hpp"
#include<algorithm>
#include<string.h>

/*
	 
		a 4次
		b 3次
		……
		共有 256 个字符，开辟256数组
*/
typedef long LongType;

//标识每个字符
struct FileInfo {
	char _ch;	
	LongType _count;
	string _code;

	FileInfo(unsigned char ch = 0)
		:_ch(ch)
		, _count(0)
	{}

	bool operator < (const FileInfo &info)
	{
		return this->_count < info._count;
	}

	FileInfo operator + (const FileInfo &info)
	{
		FileInfo tmp;
		tmp._count = this->_count + info._count;
		return tmp;
	}

	bool operator != (const FileInfo &info)const
	{
		return this->_count != info._count;
	}

};

class FileCompress {
public:
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
			_infos[i]._ch = i;
	}

public:
	bool Compress(const char* filename)
	{
		//1.打开文件，统计每个字符出现次数
		assert(filename);
		FILE* fOut = fopen(filename,"r");
		assert(fOut);

		long long chSize = 0;
		char ch = fgetc(fOut);
		while (ch != EOF)
		{
			++chSize;
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fOut);
		}

		//2.生成对应的Huffman编码
		HuffmanTree<FileInfo, NodeCompare<FileInfo>> tree;
		FileInfo invalid;						//指定非法值，_count为0
		tree.Create(_infos, 256, invalid);

		string Ins;
		Ins.clear();
		_GenerateHuffmanCode(tree.GetRoot());

		//3.写入压缩文件
		string compressfile = filename;
		compressfile += ".huffman";
		FILE *fInCompress = fopen(compressfile.c_str(), "wb");
		assert(fInCompress);

		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		int index = 0;
		unsigned char Inch = 0;
		while (ch != EOF)
		{
			string &code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				Inch <<= 1;
				if (code[i] == '1')
				{
					Inch |= 1;
				}
				if (++index == 8)
				{
					fputc(Inch, fInCompress);
					index = 0;
					Inch = 0;
				}
			}
			ch = fgetc(fOut);
		}
		//处理最后一个不满1个Byte的单元
		if (index != 0)
		{
			Inch <<= (8 - index);
			fputc(Inch, fInCompress);
		}

		//4.写配置文件
		string configfile = filename;
		configfile += ".cfig";
		FILE *fInConfig = fopen(configfile.c_str(), "wb");
		assert(fInConfig);

		string str;	//接受每个字符的_count
		
		//配置文件的第一行:> 文本总的chSize
		_itoa(chSize, (char*)str.c_str(), 10);
		fputs(str.c_str(), fInConfig);
		fputc('\n', fInConfig);

		for (size_t i = 0; i < 256; ++i)
		{
			string Inconfig;
			if (_infos[i]._count > 0)
			{
				Inconfig += _infos[i]._ch;
				Inconfig += ',';
				Inconfig += _itoa(_infos[i]._count,(char*)str.c_str(), 10);
				Inconfig += '\n';
			}
			fputs(Inconfig.c_str(), fInConfig);
			str.clear();
		}
		fclose(fOut);
		fclose(fInCompress);
		fclose(fInConfig);
		return true;
	}

	bool UnCompress(const char *filename)
	{
		//1.读取配置文件信息
		string configfile = filename;
		configfile += ".cfig";
		FILE *fOutConfig = fopen(configfile.c_str(), "rb");
		assert(fOutConfig);

		string line;
		long long chSize = 0;
		
		//配置文件的第一行:> 文本总的chSize
		ReadLine(fOutConfig, line);
		chSize += atoi(line.c_str());
		line.clear();

		while (ReadLine(fOutConfig,line))
		{
			if (!line.empty())
			{
				unsigned char ch = line[0];
				_infos[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
			else
			{
				line = '\n';
			}
		}

		//2.构造哈夫曼树
		HuffmanTree<FileInfo, NodeCompare<FileInfo>> tree;
		FileInfo invalid;						//指定非法值，_count为0
		tree.Create(_infos, 256, invalid);
		_GenerateHuffmanCode(tree.GetRoot());

		//3.解压文件
		string compressfile = filename;   //读压缩文件
		compressfile += ".huffman";
		FILE *fOutCompress = fopen(compressfile.c_str(), "rb");
		assert(fOutCompress);

		string uncompressfile = filename;    //写解压缩文件
		uncompressfile += ".uncompress";
		FILE *fInUncompress = fopen(uncompressfile.c_str(), "wb");
		assert(fInUncompress);

		char ch = fgetc(fOutCompress);
		HuffmanTreeNode<FileInfo>* cur = tree.GetRoot();
		int pos = 8;
		while (1)
		{
			//叶子节点
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fInUncompress);
				cur = tree.GetRoot();
				if(--chSize==0)
					break;
			}
			--pos;
			if (ch & (1 << pos))	//pos的最高位1则 右孩子
				cur = cur->_right;
			else
				cur = cur->_left;
			if (pos == 0)
			{
				ch = fgetc(fOutCompress);
				pos = 8;
			}
		}
		fclose(fOutConfig);
		fclose(fOutCompress);
		fclose(fInUncompress);
		return true;
	}
	
protected:
	void _GenerateHuffmanCode(HuffmanTreeNode<FileInfo>* root)
	{
		if (root == NULL)
			return;
		_GenerateHuffmanCode(root->_left);
		_GenerateHuffmanCode(root->_right);

		if (root->_left == NULL && root->_right == NULL)
		{
			HuffmanTreeNode<FileInfo>* cur = root;
			HuffmanTreeNode<FileInfo>* parent = cur->_parent;

			string& code = _infos[(unsigned char)cur->_weight._ch]._code;

			while (parent)
			{
				if (parent->_left == cur)
					code += '0';
				else
					code += '1';
				
				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(),code.end());
		}
	}

	bool ReadLine(FILE *fOut, string &str)
	{
		char ch = fgetc(fOut);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n')
		{
			str += ch;
			ch = fgetc(fOut);
		}
		return false;
	}
private:
	FileInfo _infos[256];
};
