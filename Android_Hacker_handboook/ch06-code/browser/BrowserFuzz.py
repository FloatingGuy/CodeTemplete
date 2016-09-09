#!/bin/python
#
# BrowserFuzz
#
# Amir Etemadieh
# Joshua J. Drake
#
# Requires twisted
#
# # apt-get install python-twisted
#

from twisted.web import server, resource
from twisted.internet import reactor

from generate import generate_page

import threading, time, subprocess, os, os.path, sys

LISTEN_HOST = "10.0.10.10"
LISTEN_PORT = 31337

# global init
if not os.path.exists('crashes'):
    os.mkdir('crashes', 0700)

#
# The main BrowserFuzz class. This implements the main logic including:
# 
# 1. Navigating the Android Browser to our server
# 2. Checking to see if the page load crashed the browser
# 3. Optionally killing the browser off
# 4. GOTO 1
#
class BrowserFuzz(threading.Thread):
    def __init__(self, server):
        super(BrowserFuzz, self).__init__()
        self.keep_going = True
        self.attempts = 0
        self.server = server
    
    def stop(self):
        self.keep_going = False
    
    def run(self):
        while self.keep_going:
            self.execute_test()
    
    def execute_test(self):
        self.attempts += 1
        sys.stdout.write("[*] Executing attempt # %d\033[K\r" % self.attempts)
        sys.stdout.flush()
        
        subprocess.Popen([ 'adb', 'logcat', '-c' ]).wait() # clear log

        # send the browser to our server
        tmpuri = "fuzzyou?id=%d" % (time.time())
        output = subprocess.Popen([ 'adb', 'shell', 'am', 'start',
            '-a', 'android.intent.action.VIEW',
            '-d', 'http://%s:%d/%s' % (LISTEN_HOST, LISTEN_PORT, tmpuri),
            '-e', 'com.android.browser.application_id', 'wooo',
            'com.android.chrome'
        ], stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()[0]
        
        time.sleep(60)  # give the device time to process (and hopefully crash)
        
        # see if the browser crashed
        log = subprocess.Popen([ 'adb', 'logcat', '-d' ], # dump
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()[0]
         
        # see if it crashed
        if log.find('SIGSEGV') != -1:
            crashfn = os.path.join('crashes', tmpuri)
            print "    CRASH!! Saving page/log %s" % crashfn
            with open(crashfn, "wb") as f:
                f.write(self.server.page)
            with open(crashfn + '.log', "wb") as f:
                f.write(log)

#
# Our HTTP serverthat hands out generated HTML pages
#
class FuzzServer(resource.Resource):
    isLeaf = True
    page = None
    def render_GET(self, request):
        path = request.postpath[0]
        if path == "favicon.ico":
            request.setResponseCode(404)
            return "Not found"
        self.page = generate_page()
        return self.page

if __name__ == "__main__":
    # Start the HTTP server
    server_thread = FuzzServer()
    reactor.listenTCP(LISTEN_PORT, server.Site(server_thread))
    threading.Thread(target=reactor.run, args=(False,)).start()
    
    # Start the fuzzing
    device_thread = BrowserFuzz(server_thread)
    device_thread.setDaemon(True)
    device_thread.start()
    
    try:
        while device_thread.keep_going:
            time.sleep(1)
    except (KeyboardInterrupt, SystemExit):
        print "[!] Got ^C, cleaning up..."
        device_thread.stop()
        device_thread.join()

    reactor.stop()

