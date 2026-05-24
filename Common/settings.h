#ifndef SETTINGS_H
#define SETTINGS_H
// return code format: [success(1 bit)][error level(2 bits)][module(5 bits)][class(8 bits)][code(16 bits)]
#define RETURN_SUCCESS_SHIFT 31
#define RETURN_ERROR_LEVEL_SHIFT 29
#define RETURN_MODULE_SHIFT 24
#define RETURN_CLASS_SHIFT 16
#define RETURN_CODE_SHIFT 0

#endif
