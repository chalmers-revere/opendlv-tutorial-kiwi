all: cluonDataStructures_pb2.py opendlv_standard_message_set_v0_9_6_pb2.py

cluonDataStructures.proto: cluonDataStructures.odvd
	cluon-msc --proto --out=cluonDataStructures.proto cluonDataStructures.odvd

cluonDataStructures_pb2.py: cluonDataStructures.proto
	protoc --python_out=. cluonDataStructures.proto

opendlv_standard_message_set_v0_9_6.proto: opendlv-standard-message-set-v0.9.6.odvd
	cluon-msc --proto --out=opendlv_standard_message_set_v0_9_6.proto opendlv-standard-message-set-v0.9.6.odvd

opendlv_standard_message_set_v0_9_6_pb2.py: opendlv_standard_message_set_v0_9_6.proto
	protoc --python_out=. opendlv_standard_message_set_v0_9_6.proto

clean:
	rm -f *_pb2.py *pyc *.proto 
