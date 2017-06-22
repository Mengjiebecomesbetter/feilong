/**
 * IBM (C) Copyright 2017 Apache License Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "smSocket.h"
#include "vmapiImageDefinition.h"
#include "smapiTableParser.h"

#define PARSER_TABLE_NAME     Image_Definition_Async_Updates_Layout
#define OUTPUT_STRUCTURE_NAME vmApiImageDefinitionAsyncUpdatesOutput

/**
 * Image_Definition_Async_Updates SMAPI interface
 */
int smImage_Definition_Async_Updates(struct _vmApiInternalContext* vmapiContextP, char * userid,
    int passwordLength, char * password, char * targetIdentifier, int keyValueCount,
    char ** keyValueArray,
    vmApiImageDefinitionAsyncUpdatesOutput ** outData) {

    const char * const functionName = "Image_Definition_Async_Updates";
    int tempSize;
    int keyValueSize = 0;
    char * cursor;
    int i;
    int rc;

    int inputSize = 4 + 4 + strlen(functionName) + 4 + strlen(userid) + 4
            + passwordLength + 4 + strlen(targetIdentifier);
    for (i = 0; i < keyValueCount; i++) {
        inputSize += strlen(keyValueArray[i]) + 1;
        keyValueSize += strlen(keyValueArray[i]) + 1;
    }
    char * inputP = 0;
    char * smapiOutputP = 0;
    char line[LINESIZE];

    // Build SMAPI input parameter buffer
    if (0 == (inputP = malloc(inputSize)))
        return MEMORY_ERROR;
    cursor = inputP;
    PUT_INT(inputSize - 4, cursor);
    tempSize = strlen(functionName);

    PUT_INT(tempSize, cursor);
    memcpy(cursor, functionName, tempSize);
    cursor += tempSize;

    tempSize = strlen(userid);  // UserId 1..8 or 0..8 chars
    PUT_INT(tempSize, cursor);
    if (tempSize > 0) {
        memcpy(cursor, userid, tempSize);
        cursor += tempSize;
    }

    tempSize = passwordLength;  // Password 1..200 or 0..200 chars
    PUT_INT(tempSize, cursor);
    if (tempSize > 0) {
        memcpy(cursor, password, tempSize);
        cursor += tempSize;
    }

    tempSize = strlen(targetIdentifier);  // Target identifier 1..8
    PUT_INT(tempSize, cursor);
    memcpy(cursor, targetIdentifier, tempSize);
    cursor += tempSize;

    for (i = 0; i < keyValueCount; i++) {  // Keyword = value terminated strings
        tempSize = strlen(keyValueArray[i]) + 1;
        memcpy(cursor, keyValueArray[i], tempSize);
        cursor += tempSize;
    }

    // This routine will send SMAPI the input, delete the input storage
    // and call the table parser to set the output in outData
    rc = getAndParseSmapiBuffer(vmapiContextP, &inputP, inputSize,
            PARSER_TABLE_NAME,  // Integer table
            TO_STRING(PARSER_TABLE_NAME),  // String name of the table
            (char * *) outData);
    return rc;
}
