##/ Donate: 15Hnv9T4fr9uzKMNgW5Vbop52Aq9YwSYvz
#Segment Name Map
#    -   ->    a
#   | |  ->  f   b 
#    -   ->    g
#   | |  ->  e   c
#    -   ->    d
#
#
#
import os.path
import numpy as np 

segs = int(input("Number of segments,including COM/BP: "))
digits = int(input("Number of digits including half digit: "))
com = int(input("COM/BP, should be the same used to get segment id running: "))

segNames = ['a','b','c','d','e','f','g','dp','col']# if you change this you will have to manually change all refs to pinMap
digit = [None]*segs
seg = [None]*segs
pymap = []
pyhd = []
pysc = {}
halfD = 0
hMiscVars = ''
cppMiscVars = ''
hSpecials = ''
cppSpecials = ''



def specSegs():
	global pysc,segName,cppSpecials,hSpecials
	try:
		pysc['neg']
	except KeyError:
		pysc['neg'] = pymap[halfD, segNames.index('g')]
	for i in pysc:
		hSpecials += '    uint8_t '
		hSpecials += i
		hSpecials += ';\n'
		cppSpecials += '  '+i
		cppSpecials += ' = '+str(pysc[i])
		cppSpecials += ';\n'

def setMiscVars():
	global digits,segs,halfD,hMiscVars,cppMiscVars
	mv = maxValue(digits,halfD)
	hMiscVars = '  uint8_t halfD;\n'## this is top if not += instead of =
	cppMiscVars = '  halfD = '
	if(halfD == 1):
		cppMiscVars += '1'
	else:
		cppMiscVars += '0'
	cppMiscVars += ';\n'
	hMiscVars += '    int32_t negMax;\n'
	cppMiscVars += '  negMax = '
	if(halfD):
		cppMiscVars += str(int(mv/20))
	else:
		cppMiscVars += str(int(mv/10))
	cppMiscVars += ';\n'
	hMiscVars += '    uint8_t digits;\n'
	cppMiscVars += '  digits = '+str(digits)+';\n'
	hMiscVars += '    uint8_t segs;\n'
	cppMiscVars += '  segs = '+str(segs)+';\n'
	hMiscVars += '    int32_t maxValue;\n'
	cppMiscVars += '  maxValue = '+str(mv)+';\n'
	sr = segs/8;
	if((sr-int(sr))>0):
		sr = int(sr+1)
	sr = str(sr)
	hMiscVars += '    uint8_t shiftRegNum;\n'
	cppMiscVars += '  shiftRegNum = '+sr+';\n'
	hMiscVars += '    uint8_t displayArray['+sr+'];\n'

def generatePinMap(pymap):
	global digits, segNames
	pinmap = 'uint8_t _pinmap['+str(digits)+']['+str(len(segNames))+'] = {\n  {'#create the variable stored in program memory
	for t,i in enumerate(pymap):
		tmp = ''
		cmnt = '// digit:'+str(t)+'  '
		for d, s in enumerate(i):
			tmp += str(s)
			cmnt += str(segNames[d])
			if(d<len(segNames)-1):
				tmp+=','
				cmnt += ','
		pinmap += tmp +'}'
		if(t < digits-1):
			pinmap += ', '+cmnt+'\n  {'
		else:
			pinmap += cmnt+'\n'
	pinmap += '}; \n\n'
	return pinmap




def pyMapper():
	global pymap,pyhd,pysc,com,segs
	pymap = np.zeros((digits,9),'uint8')
	pymap.fill(com)
	pyhd = []
	pysc = {}
	i = 0
	while i < segs:
		if digit[i] == 0:
			pysc[seg[i]] = i
		else:
			si = 0
			for s in segNames:
				if (s == seg[i]):#
					pymap[digit[i]-1,si] = i #get this to order correctly
					break
				si += 1
		i += 1


def checkDigit(dig):#checks the input digit for duplicate segs
	global halfD
	if(dig>0):
		lastDig = 0
		freq = []
		t = 0
		while True:
			try:
				lastDig = digit.index(dig,lastDig) #finds the next index of the digit 
				t += 1
				for nam in segNames:
					if(seg[lastDig] == nam):
						freq += [nam]
				lastDig += 1 #increments the beginning of index up one so it doesn't get stuck returning first value
			except ValueError:
				break
		if(t):# makes sure if the digit even existed
			for nam in segNames: # counts the occurances of the names in segment lists
				num = freq.count(nam)
				if(num == 1): # if it equals exactly one do nothing since that is how it should be
					pass
				elif(num>1):# if it finds more than one print the duplicates then promt for new values
					print('the following were duplicated you cannot have two outputs connected to the same pin')
					tmp = []
					lastDig = 0
					while True:
						try:
							lastDig = digit.index(dig,lastDig)
							if(seg[lastDig]==nam):
								tmp += [lastDig]
							lastDig += 1
						except ValueError:
							break
					for bla in tmp:
						displaySeg(bla)
					for bla in tmp:
						inputSeg(bla)
				elif(num == 0):# if there are none propmt to see if they meant to not include them
					if((dig !=1 or (dig == 1 and len(freq)>4))and nam != 'col'):
						tmp = input('does '+str(dig)+' digit have a "'+nam+'" segment(y/n):')#ask if the digit even has that segment
						if(tmp =='y'): #if it does supply the proper pin it is connected to
							bla = int(input('what output pin number is it 0-'+str(segs-1)+':'))
							while(bla> segs-1 or bla < 0):# makes sure that it is a valid input
								bla = int(input('put in a valid output pin value: 0-'+str(segs-1)+':'))
							print('was:')
							displaySeg(bla)
							digit[bla] = dig
							seg[bla] = nam
							print('is now ->')
							displaySeg(bla)
			if(dig == 1 and len(freq)<5):
				tmp = 0
				bla = 0
				halfD = 1
				while True:
					try:
						bla = digit.index(dig,bla)
						while(not((seg[bla] == 'b')or(seg[bla] == 'c') or (seg[bla] =='dp') or (seg[bla] =='col'))):
							print('half digits can only have b,c,dp,or col segments')
							displaySeg(bla)
							seg[bla] = input('change segment to b,c,dp,or col:')
							print('updated to:')
							displaySeg(bla)
						bla +=1
					except ValueError:
						break
		else:
			print('can only check digits that exist')
	else:# checks for duplicates in the special symbols of digit 0
		lastDig = 0
		tmp = []
		while True:
			try:
				tmp += [seg[digit.index(dig,lastDig)]]
				lastDig += 1
			except ValueError:
				break
		for bla in tmp:
			while(seg.count(bla)>1):
				print('you have more than one special character witht the same name')
				crnt = seg.index(bla)
				displaySeg(crnt)
				seg[crnt] = input('change name of segment:')
				displaySeg(crnt)

def maxValue(digits, halfD):
	a = 1
	for i in range(0,digits, 1):
		a *=10
	if (halfD > 0):
		a = (a/10)+((a/10)-1)
	else:
		a -= 1
	return int(a)



def displayDigit(dig):
	lastDig = 0
	while True:
			try:
				lastDig = digit.index(dig,lastDig)
				print(str(lastDig)+' Digit:'+str(digit[lastDig])+'  Segment:'+seg[lastDig])
				lastDig += 1
			except ValueError:
				break


def displaySeg(segm):
	print(str(segm)+' | Digit:'+str(digit[segm]))
	print(str(segm)+' | Segment:'+str(seg[segm]))

def inputSeg(pinNum):
	dig = input(str(pinNum)+' | Digit:')
	if(dig):
		while True:
			try:
				dig = int(dig)
				break
			except ValueError:
				print('must be an integer!')
				dig = input(str(pinNum)+' | Digit:')
	else:
		dig = 0
	segm = input(str(pinNum)+' | Segment:')
	while (len(segm)<=0):
		print('you must put in a segment letter or name')
		segm = input(str(pinNum)+' | Segment:')
	if(dig>0):		#if it is not a special symbol 
		tmp = 0
		while tmp<1:
			for nam in segNames:
				if(segm == nam):
					tmp +=1;
			if(tmp<1):
				print('only valid inputs for digits are a-g, dp, col')
				segm = input(str(pinNum)+' | Segment:')
	seg[pinNum] = segm
	digit[pinNum] = dig


print("Naming rules:")
print("digits ONE indexed starting from the left most(even if its a half digit)")
print("a = top center, b = top right... ...f = top left, g = center, dp = decimal, col = colon")
print("special symbols & negative are part of digit 0 ")
print("special symbols must start with character and cannot equal any names in the pervious lines")
print("colons are part of the digit immediately to its left")
print("name negative sign if applicable as 'neg'")
print("half digit will automatically be detected if digit one only has values for either the b,c,dp, and/or col")
print("Output Pin #| Digit/Segment")

for i in range(0,segs,1):
	if(i != com):
		inputSeg(i)
	else:
		digit[i] = 0
		seg[i] = 'com'
		displaySeg(i)

i = 0
tmp = []
while digit.count(i):
	tmp += [digit.count(i)] 
	i += 1
i = 0
for bla in tmp:
	if(i == 0):
		print('detected '+str(bla)+' special symbol segments')
		checkDigit(i)
	else:
		if(bla < 5):
			print('detected '+str(bla)+'  segments in digit '+str(i)+' assumed half digit since it has less than 5 segments')
		else:
			print('detected '+str(bla)+'  segments in digit '+str(i))
		checkDigit(i)
	i += 1

pyMapper()
dirpath = os.path.dirname(os.path.abspath(__file__))
pinmap = generatePinMap(pymap)
specialVars = specSegs()
miscVars = setMiscVars()

#cpp = '#ifndef LCDvars_h\n#define LCDvars_h\n\n'+pinmap+specialVars+miscVars+'\n\n#endif'
cppPris = open(dirpath+'\\shiftLCD.cppPristine','r').read()
hPris = open(dirpath+'\\shiftLCd.hPristine','r').read()
cidx = cppPris.find('shiftLCD::shiftLCD')
cidx2 = cppPris.rfind('}',0,cppPris.find('void shiftLCD::')-1)
cpp = cppPris[0:cidx]+pinmap+cppPris[cidx:cidx2]+cppMiscVars+cppSpecials+cppPris[cidx2:]
hidx = hPris.find('private:')
h = hPris[0:hidx]+hMiscVars+hSpecials+hPris[hidx:]

cppfile = open(dirpath+'\\shiftLCD.cpp','+w')
hfile =   open(dirpath+'\\shiftLCD.h','+w')

cppfile.write(cpp)
cppfile.close()
hfile.write(h)
hfile.close()

