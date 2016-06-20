# Dreamcast Utilities
# -------------------
#
# Runs dc-tool using certain environment variables and logs to a file.
#
import os
import subprocess
import sys
import time


def get_dreamcast_ip():
    if "DREAMCAST_IP" not in os.environ:
        raise Exception("DREAMCAST_IP not defined in environment variables!")
    return os.environ["DREAMCAST_IP"]


def get_dctool_path():
    if "DC_TOOL_PATH" not in os.environ:
        raise Exception("DC_TOOL_PATH not defined in environment variables!")
    return os.environ["DC_TOOL_PATH"]


def get_log_path():
    if "DC_TEST_LOG" in os.environ:
        return os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            "logfile.txt")
    else:
        return None


def log_arguments(arg):
    path = get_log_path()
    if path:
        import datetime
        with open(path, 'a') as log:
            log.write("%s\tdctest %s\n" % (datetime.datetime.now(), arg))
    print("dctest %s" % arg)


class DcToolRunner(object):

    def __init__(self, binary_file):
        self.args = [get_dctool_path(), "-t", get_dreamcast_ip(),
                     "-x", binary_file]
        self.proc = subprocess.Popen(
            self.args,
            bufsize=1,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            universal_newlines=True
        )
        self.errors = []
        self.tests_completed = False

    def run(self):
        while self.proc.poll() is None:
            line = self.proc.stdout.readline()
            if "@DC_TEST_COMPLETE" in line:
                self.tests_completed = True
            if "@DC_TEST_ERROR" in line:
                self.errors.append(line)
            sys.stdout.write(line)


class DreamcastCooler(object):
    """Make sure we give the Dreamcast some time to rest."""

    def __init__(self):
        self.start = time.time()

    def cool_down(self):
        time.sleep(2)
        print("dc-tool finished in %d seconds..." % self.elapsed_time())
        print("""
        Break time!
                       [@] <* zzz...
            """)
        while self.elapsed_time() < 6:
            time.sleep(1)

    def elapsed_time(self):
        return time.time() - self.start
