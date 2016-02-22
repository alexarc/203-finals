# HTTP Server
## Overview
This assignment's goal is to write a simple, albeit functional HTTP server.
A HTTP server is the software in charge of delivering documents, most often web pages, upon request of a HTTP client, or "user agent".

## Tier 1

- Process and answer a set of HTTP queries, given a document storage described in a local text file. 
Produce outputs as text files of matching names. Produce common error messages for simple cases, such as when a file is missing, or the URL is incorrect.

## Tier 2

- Use the winsock interface to listen for requests from the client. Answer responses appropriately.
- Use the file system interface to provide directory listings when no index file is present.

## Tier 3
 
- Support setting and retrieving cookies
- Support caching documents
- Look for file extensions to send appropriate content-type
- Support multiple incoming connections
