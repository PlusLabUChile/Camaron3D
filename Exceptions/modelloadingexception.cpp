#include "modelloadingexception.h"

ModelLoadingException::ModelLoadingException(std::string filepath,
											 std::string message):
	UnknownExtensionException(filepath),
	ExceptionMessage(message)
{

}
ModelLoadingException::~ModelLoadingException(){}
