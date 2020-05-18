from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
import re

class PySysTest(BaseTest):
	def execute(self):
		correlator = CorrelatorHelper(self, name='testcorrelator')
		correlator.start(logfile='testcorrelator.log', config=self.input+'/config.yaml')
		correlator.injectEPL(filenames=['ConnectivityPluginsControl.mon'], filedir=PROJECT.APAMA_HOME+'/monitors')
		correlator.injectEPL(filenames=['ConnectivityPlugins.mon'], filedir=PROJECT.APAMA_HOME+'/monitors')
		correlator.injectEPL(filenames=['test.mon'])
		correlator.flush() 

	def validate(self):
		self.assertLineCount('after.txt', expr='Towards Transport', condition='==5')
		self.assertLineCount('testcorrelator.log', expr=r'ERROR .*Dropping message because it exceeds the maximum size of 5000 bytes \(message was [0-9]* bytes\)', condition='==6')
