rsync -va \
      -e "ssh -ax -o ClearAllForwardings=yes -c blowfish -T " \
      $@
#rsyncCCSE slpiao@162.105.125.59:/iodisk5/home/slpiao/TESTS_MANCIP/modipsl ./
#rsyncCCSE modipsl slpiao@162.105.125.59:/iodisk5/home/slpiao/TESTS_MANCIP
#rsyncCCSE modipsl slpiao@162.105.125.59:~slpiao/TESTS_MANCIP