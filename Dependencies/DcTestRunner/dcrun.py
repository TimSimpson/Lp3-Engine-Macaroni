# Dreamcast Demo Runner
# ---------------------
#
# Runs dc-tool. That's it! Just a helper script which reuses environment vars.
#
from dcutils import *


def run_test(binary_file):
    runner = DcToolRunner(sys.argv[1])
    print(" ".join(runner.args))
    runner.run()


if __name__=="__main__":
    if len(sys.argv) != 2:
        raise Exception("Usage: cdrun.py dc_elf_file")
    elf_file = sys.argv[1]
    log_arguments(elf_file)
    run_test(elf_file)
