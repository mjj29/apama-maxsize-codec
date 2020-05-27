# apama-maxsize-codec
Apama connectivity codec to enforce a maximum message size and drop any messages above that

## Supported Apama version

This works with Apama 10.1 or later

## Building the plugin

In an Apama command prompt on Linux run:

    mkdir -p $APAMA_WORK/lib
    g++ -std=c++11 -o $APAMA_WORK/lib/libconnectivity-maxsize-codec.so -I$APAMA_HOME/include -L$APAMA_HOME/lib -lapclient -I. -shared -fPIC MaxSizeCodec.cpp

On Windows run:

    g++ -std=c++11 -o %APAMA_WORK%\lib\connectivity-maxsize-codec.dll -I%APAMA_HOME%\include -L%APAMA_HOME%\lib -I. -shared MaxSizeCodec.cpp -lapclient

## Building using Docker

There is a provided Dockerfile which will build the plugin, run tests and produce an image which is your base image, plus the MaxSize plugin. Application images can then be built from this image. To build the image run:

    docker build -t apama_with_maxsize_plugin .

By default the public docker images from Docker Store for 10.5 will be used. To use another version run:

    docker build -t apama_with_maxsize_plugin --build-arg APAMA_VERSION=10.1 .

To use custom images from your own repository then use:

    docker build -t apama_with_maxsize_plugin --build-arg APAMA_BUILDER=builderimage --build-arg APAMA_IMAGE=runtimeimage .

## Running tests

To run the tests for the plugin you will need to use an Apama command prompt to run the tests from within the tests directory:

    pysys run

## Using the max size codec

As a codec in a connectivity chain you will need to first import the plugin into your configuration:

    connectivityPlugins:
	   maxSizeCodec:
        libraryName: connectivity-maxsize-codec
		  class: MaxSizeCodec

You can now use the max size codec in a chain definition:

    maxSizeChain:
	    - apama.eventMap
       - mapperCodec:
		    # ... mapping configuration
		 - jsonCodec
		 - stringCodec
		 - maxSizeCodec:
			   maxSizeBytes: 20971520
		 - someTransport

The MaxSize codec will check messages going towards the transport and enforce a maximum size. Any messages above that size will be dropped. Only messages towards the transport with a buffer-type payload will be checked, so it must be below something like the string codec. Setting the maxSizeBytes to 0 will also disable checks.

