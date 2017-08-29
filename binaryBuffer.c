#include "binaryBuffer.h"
#ifndef BUFFER_SIZE_INCREASE
#define BUFFER_SIZE_INCREASE 10
#endif 
#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 1500
#endif 

/* constructor */
binaryBufferClass * new_binaryBufferObj(usint bufferSize) {

	binaryBufferClass* self = (binaryBufferClass*)malloc(sizeof(binaryBufferClass));

	/* free object*/
	self->free = &binaryBufferClass_detor;

	/* print object*/
	self->print = &print_binaryBufferObj;

	/* create metaData2BitStream struct*/
	self->create = &create_metaData2BitStream;

	/* byte maniplation */
	self->add = &add_Binary2rawMessage;
	self->pop = &pop_Binary2rawMessage;

	/* reset attributes */
	self->reset = &reset_attributes;

	/* setter methods */
	self->set_messageID = &set_messageID;
	self->set_WN = &set_WN;
	self->set_ToW = &set_ToW;
	//self->set_bufferSize = &set_bufferSize;
	self->set_messageLength = &set_messageLength;
	self->set_sequenceNumber = &set_sequenceNumber;

	/*set default attributes*/
	self->messageID = 0;
	self->WN = 0;
	self->ToW = 0;
	self->bufferSize = bufferSize;										// buffer size
	self->messageLength = 0;											// message length in byte
	self->rawMessage = (Byte*)malloc(sizeof(Byte)*bufferSize);			// all of the raw incoming message
	self->sequenceNumber = 0;

	return self;
}
/* destructor */
static void binaryBufferClass_detor(binaryBufferClass * self){
	free(self->rawMessage);
	free(self);
}
/* Static methods */
static void print_binaryBufferObj(binaryBufferClass * self){

	printf("\nData of the binaryBufferObj\n");
	printf("messageID: %d \n", self->messageID);
	printf("WN: %d \n", self->WN);
	printf("ToW: %d \n", self->ToW);
	printf("bufferSize: %d \n", self->bufferSize);
	printf("messageLength: %d \n", self->messageLength);
	printf("sequenceNumber: %d \n", self->sequenceNumber);
	printf("rawMessage: ");
	for (int i = 0; i < self->messageLength; i++){
		printf("%d\t, ", self->rawMessage[i]);
	}
	printf("\n");
}
static void set_messageID(binaryBufferClass * self, usint messageID){
	self->messageID = messageID;
}
static void set_WN(binaryBufferClass * self, usint WN){
	self->WN = WN;
}
static void set_ToW(binaryBufferClass * self, usint ToW){
	self->ToW = ToW;
}
/*static void set_bufferSize(binaryBufferClass * self, usint bufferSize){
	self->bufferSize = bufferSize;
}*/
static void set_messageLength(binaryBufferClass * self, usint messageLength){
	self->messageLength = messageLength;
}
static void set_sequenceNumber(binaryBufferClass * self, usint sequenceNumber){
	self->sequenceNumber = sequenceNumber;
}
static void reset_attributes(binaryBufferClass * self){
	self->messageID = 0;
	self->WN = 0;
	self->ToW = 0;									
	self->messageLength = 0;											
	self->sequenceNumber = 0;
}
static metaData2BitStream_t create_metaData2BitStream(binaryBufferClass * self){
	metaData2BitStream_t* output = (metaData2BitStream_t*)malloc(sizeof(metaData2BitStream_t));

	output->messageID = self->messageID;
	output->WN = self->WN;
	output->ToW = self->ToW;
	output->TimeFormat.time_in_secs_from_1980 = 0;
	output->TimeFormat.time_of_clock = 3601;
	output->TimeFormat.ToW = self->ToW;
	output->TimeFormat.WN = self->WN;
	output->messageLength = self->messageLength;
	output->sequenceNumber = self->sequenceNumber;
	
	// Check this part with Mark
	output->rawMessage = (Byte*)malloc(sizeof(Byte)*self->messageLength);
	for (int i = 0; i < self->messageLength; i++){
		output->rawMessage[i] = self->rawMessage[i];
	}

	return *output;
}
static void add_Binary2rawMessage(binaryBufferClass * self, Byte newByte){

	if (self->messageLength + 1 > self->bufferSize){
		self->rawMessage = (Byte *)realloc(self->rawMessage, sizeof(Byte)*(self->bufferSize + BUFFER_SIZE_INCREASE));
		if (self->bufferSize + BUFFER_SIZE_INCREASE < MAX_BUFFER_SIZE){
			self->bufferSize += BUFFER_SIZE_INCREASE;
		}
		else{
			#ifdef BINARY_BUFFER_DEBUG_MODE_ON
				printf("\WARNING! Stack overflow. The binar buffer is full and overflowed. WARNING!\n");
			#endif
			return;
		}
		
	}
	self->rawMessage[self->messageLength] = newByte;
	self->messageLength++;
}
static void pop_Binary2rawMessage(binaryBufferClass * self){
	self->messageLength--;
}
