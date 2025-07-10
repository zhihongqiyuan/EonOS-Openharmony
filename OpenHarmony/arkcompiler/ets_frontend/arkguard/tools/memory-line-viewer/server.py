#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2025 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import logging
import argparse
import http.server
import mimetypes
import os
import socketserver
import threading
import time
import urllib
import webbrowser

# Configure logging to display timestamps, log levels, and messages
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


class StaticFileServerHandler(http.server.SimpleHTTPRequestHandler):
    """
    Custom HTTP request handler for serving static files and handling custom endpoints.
    """

    def do_GET(self):
        """
        Override the do_GET method to handle custom endpoints:
        - /get_filename: Returns the filename of the loaded file.
        - /download: Handles file downloads based on query parameters.
        - Other paths are handled by the parent class (serving static files).
        """
        if self.path == "/get_filename":
            self.handle_get_filename()
        elif self.path.startswith("/download"):
            self.handle_download()
        else:
            super().do_GET()

    def guess_type(self, path):
        """
        Override the guess_type method to customize MIME type detection:
        - Set MIME type for .js files to 'application/javascript'.
        - Default to 'application/octet-stream' if unknown.
        """
        base, ext = os.path.splitext(path)
        mime_type = mimetypes.guess_type(path)[0] or "application/octet-stream"
        if ext.lower() == ".js":
            mime_type = "application/javascript"
        return mime_type

    def handle_download(self):
        """
        Handle file downloads:
        - Extract the 'filename' parameter from query strings.
        - Serve the requested file if it exists.
        - Return appropriate HTTP responses for success or errors.
        """
        query_params = urllib.parse.parse_qs(urllib.parse.urlparse(self.path).query)
        filename = query_params.get("filename", [None])[0]
        if filename:
            file_to_load = os.path.join(
                os.path.dirname(self.server.file_to_load), filename
            )
            if os.path.exists(file_to_load):
                file_to_load = os.path.abspath(file_to_load)
                self.send_response(200)
                self.send_header("Content-Type", "application/octet-stream")
                self.send_header(
                    "Content-Disposition", f'attachment; filename="{filename}"'
                )
                self.end_headers()

                with open(file_to_load, "rb") as file:
                    self.wfile.write(file.read())
            else:
                self.send_error(404, "File not found")
        else:
            self.send_error(400, "Filename parameter is required")

    def handle_get_filename(self):
        """
        Handle the /get_filename endpoint:
        - Return the basename of the loaded file as plain text.
        """
        filename = os.path.basename(self.server.file_to_load)
        self.send_response(200)
        self.send_header("Content-Type", "text/plain")
        self.end_headers()
        self.wfile.write(filename.encode("utf-8"))


class SimpleServer(socketserver.ThreadingTCPServer):
    """
    Custom TCP server class to support threading and reuse of addresses.
    """

    allow_reuse_address = True

    def __init__(self, server_address, RequestHandlerClass, file_to_load):
        """
        Initialize the server with a file to load.
        """
        self.file_to_load = file_to_load
        super().__init__(server_address, RequestHandlerClass)


def open_webpage(url):
    """
    Open a URL in a new browser tab.
    """
    webbrowser.open_new_tab(url)


def start_server_with_webpage_opening():
    """
    Main function to start the HTTP server and open a webpage.
    """
    parser = argparse.ArgumentParser(description="Start a simple HTTP server.")
    parser.add_argument(
        "--port", "-p", type=int, default=9000, help="Specify the port number."
    )
    parser.add_argument("--file", type=str, help="Specify a file to handle.")
    args = parser.parse_args()

    if not args.file:
        logging.error("No file specified using --file option.")
        return

    if not os.path.exists(args.file):
        logging.error(f"The file '{args.file}' does not exist.")
        return

    server_address = ("", args.port)
    with SimpleServer(server_address, StaticFileServerHandler, args.file) as httpd:
        logging.info(f"Serving HTTP on port {args.port}...")
        logging.info(f"Loading file from path: {args.file}")
        server_thread = threading.Thread(target=httpd.serve_forever)
        server_thread.daemon = True
        server_thread.start()
        time.sleep(1)
        url = f"http://localhost:{args.port}/index.html"
        open_webpage(url)
        time.sleep(2)
        httpd.shutdown()
        httpd.server_close()


if __name__ == "__main__":
    start_server_with_webpage_opening()
