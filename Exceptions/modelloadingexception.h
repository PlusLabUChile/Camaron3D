#ifndef MODELLOADINGEXCEPTION_H
#define MODELLOADINGEXCEPTION_H
#include "Exceptions/unknownextensionexception.h"
#include "exceptionmessage.h"
class ModelLoadingException:
		public UnknownExtensionException,
		public ExceptionMessage
{
	public:
		ModelLoadingException(std::string filepath,
							  std::string message);
		virtual ~ModelLoadingException();
};

#endif // MODELLOADINGEXCEPTION_H
