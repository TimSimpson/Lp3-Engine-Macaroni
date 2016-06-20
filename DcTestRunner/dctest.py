# Dreamcast Test Runner
# ---------------------
#
# Runs dc-tool and monitors stdout to test the Dreamcast.
#
# Output format:
#   @DC_TEST_ERROR: blah - Some error line.
#   @DC_TEST_COMPLETE    - All tests executed successfully.
#
from dcutils import *


def run_test(binary_file):
    cool = DreamcastCooler()
    runner = DcToolRunner(sys.argv[1])
    runner.run()
    cool.cool_down()
    if len(runner.errors) > 0:
        print("Test errors!")
        exit(len(runner.errors))
    elif not runner.tests_completed:
        print("Test did not complete successfully.")
        exit(-1)


if __name__=="__main__":
    if len(sys.argv) != 2:
        raise Exception("Usage: dctest.py dc_elf_file")
    test_file = sys.argv[1]
    if not test_file.endswith(".exe"):
        raise Exception("Error: %s does not look like a test!" % test_file)
    log_arguments(test_file)
    run_test(test_file)
