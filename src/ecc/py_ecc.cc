#define PY_SSIZE_T_CLEAN
#include "py_ecc.h"
#include "../util/util.h"
#include "include/Python.h"

using kp = std::pair<std::string, std::string>;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

kp init_getKey(int N, string X, string Y, int A, int B) {
	kp answer;
	Py_Initialize();
	if (Py_IsInitialized()) {
	    PyObject* pModule = NULL;
	    PyObject* pFunc = NULL;
	    pModule = PyImport_ImportModule("ecc");
	    if (pModule) {
	        pFunc = PyObject_GetAttrString(pModule, "getKey");

	        PyObject* pArgs = PyTuple_New(5);

	        PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", N));
	        PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", X.data()));
	        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s", Y.data()));
	        PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", A));
	        PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", B));

	        PyObject* pReturn = PyEval_CallObject(pFunc, pArgs);
	        
	        char *key1, *key2;

	        PyArg_ParseTuple(pReturn, "ss", &key1, &key2);

	        answer = make_pair(std::string(key1).substr(2),
	        				   std::string(key2).substr(2));
	    }
	    else
	    {
	        printf("导入Python模块失败...\n");
	    }
	}
	else
	{
	    printf("Python环境初始化失败...\n");
	}
	Py_Finalize();
	return answer;
}

kp ECC::getKey(int N, string X, string Y, int A, int B) {
	if (4 * A * A * A + 27 * B * B == 0) throw "ecc input error";
	return init_getKey(N, X, Y, A, B);
}

string ECC::encrypt(string message, int N, string X, string Y, int A, int B) {
	kp val = getKey(N, X, Y, A, B);
	int now = 0;
	message = Util::HexToBinary(message);
	val.first = Util::HexToBinary(val.first);
	for (auto& i : message)
		i = ((i - '0') ^ (val.first[now++] - '0')) + '0';
	message = Util::BinaryToHex(message);
	return message;
}

string ECC::decrypt(string message, int N, string X, string Y, int A, int B) {
	kp val = getKey(N, X, Y, A, B);
	int now = 0;
	message = Util::HexToBinary(message);
	val.first = Util::HexToBinary(val.first);
	for (auto& i : message)
		i = ((i - '0') ^ (val.first[now++] - '0')) + '0';
	message = Util::BinaryToHex(message);
	return message;
}