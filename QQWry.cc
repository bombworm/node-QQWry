#include "IPInfo.h"
#define USING_STATIC_LIBICONV
#include "iconv.h"
#pragma comment(lib, "libiconvStatic.lib")
#ifndef ICONV_CONST
# define ICONV_CONST const
#endif
////////////////////////////////Essentials Above//////////////////////////////////////////
#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace node;
using namespace v8;

iconv_t conv_;

namespace{
	class QQWry: ObjectWrap
	{
	private:
		static Persistent<FunctionTemplate> constructor_template;
		CIPInfo m_IPInfo;
	public:
		static void Init(Handle<Object> target);

		QQWry(Handle<Object> wrapper, CIPInfo qqwry): m_IPInfo(qqwry)
		{
			Wrap(wrapper);
		}

		~QQWry()
		{
			m_IPInfo.Close();
		}

		static Handle<Value> New(const Arguments& args);
		static Handle<Value> QueryIP(const Arguments& args);
		Handle<Value> QueryIP(char *ipAddress);
	};
	Persistent<FunctionTemplate> QQWry::constructor_template;

	Handle<Value> QQWry::New(const Arguments& args)
	{
		HandleScope scope;
		String::AsciiValue path(args[0]->ToString());
		CIPInfo qqwry;
		if (FALSE == qqwry.OpenA(*path)) {
			return ThrowException(Exception::Error(String::New(
				"QQWry:  File not Found.")));
		}
		new QQWry(args.This(), qqwry);
		return args.This();
	}

	void FreeMemory(char *data, void *hint) {
		free(data);
		V8::AdjustAmountOfExternalAllocatedMemory(-(sizeof(Buffer) + (size_t) hint));
	}

	int grow(char** output, size_t* outlen, char** outbuf, size_t* outbufsz) {
	  size_t newlen;
	  char *newptr;

	  newlen = *outlen ? (*outlen * 2) : 16;
	  if ((newptr = (char*) realloc(*output, newlen))) {
		*outbufsz = newlen - *outlen;
		*outbuf = newptr + (*outbuf - *output);

		*outlen = newlen;
		*output = newptr;

		return 1;
	  }

	  return 0;
	}

	/**
	 * This function will clobber `output` and `outlen` on both success and error.
	 */
	int convert(iconv_t iv, char* input, size_t inlen, char** output, size_t* outlen) {
	  char* inbuf;
	  char* outbuf;
	  size_t outbufsz;
	  size_t inbufsz;
	  size_t rv;

	  inbufsz = inlen;
	  inbuf = input;

	  *outlen = outbufsz = 0;
	  *output = outbuf = 0;

	  // reset to initial state
	  iconv(iv, 0, 0, 0, 0);

	  // convert input
	  do {
		if (grow(output, outlen, &outbuf, &outbufsz)) {
		  rv = iconv(iv, (ICONV_CONST char **) &inbuf, &inbufsz, &outbuf, &outbufsz);
		}
		else {
		  goto error;
		}
	  }
	  while (rv == (size_t) -1 && errno == E2BIG);

	  if (rv == (size_t) -1) {
		goto error;
	  }

	  // write out shift sequence
	  rv = iconv(iv, 0, 0, &outbuf, &outbufsz);

	  if (rv == (size_t) -1) {
		if (errno != E2BIG) {
		  goto error;
		}
		if (!grow(output, outlen, &outbuf, &outbufsz)) {
		  goto error;
		}
		if (iconv(iv, 0, 0, &outbuf, &outbufsz) == (size_t) -1) {
		  goto error;
		}
	  }

	  // store length
	  *outlen = outbuf - *output;

	  // release unused trailing memory; this can't conceivably fail
	  // because newlen <= oldlen but let's take the safe route anyway
	  //
	  // realloc() may free the memory and return NULL if *outlen == 0
	  // but that's not an error, the caller should (and does) handle it
	  // graciously
	  //
	  if ((outbuf = (char*) realloc(*output, *outlen)) || *outlen == 0) {
		*output = outbuf;
	  }

	  return 1;

	error:
	  free(*output);
	  *output = 0;
	  *outlen = 0;
	  return 0;
	}

	Handle<Value> QQWry::QueryIP(char* ipAddress){
		char szLocation[512]={0};
		//char *szLocation = new char[512];
		//memset(szLocation, 0, sizeof(szLocation));
		if(m_IPInfo.QueryIPA(ipAddress, szLocation, sizeof(szLocation))){
			//printf("%s\n", szLocation);
			/*int len = strlen(szLocation);
			for(int i=0;i<len;i++){
				printf("%2x ", (byte)szLocation[i]);
			}
			printf("\n%d\n", len);
			V8::AdjustAmountOfExternalAllocatedMemory(sizeof(Buffer) + len);
			return Buffer::New(szLocation, len, FreeMemory, (void *) len)->handle_;
			*/
			char* p = strstr(szLocation, "CZ88.NET");
			if (p != NULL) p[-1] = '\0';
			int inlen = strlen(szLocation);
			size_t outlen;
			char *output;

			outlen = 0;
			output = 0;

			if (convert(conv_, szLocation, inlen, &output, &outlen)) {
				V8::AdjustAmountOfExternalAllocatedMemory(sizeof(Buffer) + outlen);
				return Buffer::New(output, outlen, FreeMemory, (void *) outlen)->handle_;
			}
			else if (errno == EINVAL) {
				return ThrowException(ErrnoException(EINVAL, "iconv", "Incomplete character sequence."));
			}
			else if (errno == EILSEQ) {
				return ThrowException(ErrnoException(errno, "iconv", "Illegal character sequence."));
			}
			else if (errno == ENOMEM) {
				V8::LowMemoryNotification();
				return ThrowException(ErrnoException(errno, "iconv", "Out of memory."));
			}
			else {
				return ThrowException(ErrnoException(errno, "iconv"));
			}
		}
		return Undefined();
	}

	Handle<Value> QQWry::QueryIP(const Arguments& args)
	{
		HandleScope scope;

		QQWry* self = ObjectWrap::Unwrap<QQWry>(args.This());
		Local<Value> arg = args[0];
		if (arg->IsString()) {
			String::Utf8Value string(arg->ToString());
			return self->QueryIP(*string);
		}
		return Undefined();
	}

	void QQWry::Init(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> t = FunctionTemplate::New(QQWry::New);

		constructor_template = Persistent<FunctionTemplate>::New(t);
		constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
		constructor_template->SetClassName(String::NewSymbol("QQWry"));

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "QueryIP", QueryIP);

		target->Set(String::NewSymbol("QQWry"),
			constructor_template->GetFunction());

		//printf("%s\n", String::AsciiValue(constructor_template->GetFunction()->ToString()));
	}

	static void init (Handle<Object> target)
	{
		QQWry::Init(target);
		conv_ = iconv_open("UTF-8", "GBK");
	}

	NODE_MODULE(QQWry, init);
}