/*
 *  metastatscommand.cpp
 *  Mothur
 *
 *  Created by westcott on 9/16/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "metastatscommand.h"


//CommandParameter(string n, string t, string o, string d, string only, string atLeast, string linked, string opt, bool m, bool r, bool i) : name(n), type(t), options(o), optionsDefault(d), chooseOnlyOneGroup(only), chooseAtLeastOneGroup(atLeast), linkedGroup(linked), outputTypes(opt),multipleSelectionAllowed(m), required(r), important(i) {}

//**********************************************************************************************************************
vector<string> MetaStatsCommand::setParameters(){	
	try {
		CommandParameter pshared("shared", "InputTypes", "", "", "shared-lcr", "none", "none","metastats",false,false,true); parameters.push_back(pshared);
        CommandParameter plcr("lcr", "InputTypes", "", "", "shared-lcr", "none", "none","metastats",false,false,true); parameters.push_back(plcr);
		CommandParameter pdesign("design", "InputTypes", "", "", "none", "none", "none","",false,true,true); parameters.push_back(pdesign);
		CommandParameter pprocessors("processors", "Number", "", "1", "", "", "","",false,false,true); parameters.push_back(pprocessors);
		CommandParameter piters("iters", "Number", "", "1000", "", "", "","",false,false); parameters.push_back(piters);
		CommandParameter pthreshold("threshold", "Number", "", "0.05", "", "", "","",false,false); parameters.push_back(pthreshold);
		CommandParameter plabel("label", "String", "", "", "", "", "","",false,false); parameters.push_back(plabel);
		CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter psets("sets", "String", "", "", "", "", "","",false,false); parameters.push_back(psets);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string MetaStatsCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "This command is based on the Metastats program, White, J.R., Nagarajan, N. & Pop, M. Statistical methods for detecting differentially abundant features in clinical metagenomic samples. PLoS Comput Biol 5, e1000352 (2009).\n";
		helpString += "The metastats command outputs a .metastats file. \n";
		helpString += "The metastats command parameters are shared, lcr, iters, threshold, groups, label, design, sets and processors.  The shared or lcr and design parameters are required, unless you have valid current files.\n";
		helpString += "The design parameter allows you to assign your groups to sets when you are running metastat. mothur will run all pairwise comparisons of the sets. It is required. \n";
		helpString += "The design file looks like the group file.  It is a 2 column tab delimited file, where the first column is the group name and the second column is the set the group belongs to.\n";
		helpString += "The sets parameter allows you to specify which of the sets in your designfile you would like to analyze. The set names are separated by dashes. THe default is all sets in the designfile.\n";
		helpString += "The iters parameter allows you to set number of bootstrap permutations for estimating null distribution of t statistic.  The default is 1000. \n";
		helpString += "The threshold parameter allows you to set the significance level to reject null hypotheses (default 0.05).\n";
		helpString += "The groups parameter allows you to specify which of the groups in your groupfile you would like included. The group names are separated by dashes.\n";
		helpString += "The label parameter allows you to select what distance levels you would like, and are also separated by dashes.\n";
		helpString += "The processors parameter allows you to specify how many processors you would like to use.  The default is 1. \n";
		helpString += "The metastats command should be in the following format: metastats(design=yourDesignFile).\n";
		helpString += "Example metastats(design=temp.design, groups=A-B-C).\n";
		helpString += "The default value for groups is all the groups in your groupfile, and all labels in your inputfile will be used.\n";
		
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string MetaStatsCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "metastats") {  pattern = "[filename],[distance],[group],metastats"; } 
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "MetaStatsCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
MetaStatsCommand::MetaStatsCommand(){	
	try {
		abort = true; calledHelp = true;
		setParameters();
		vector<string> tempOutNames;
		outputTypes["metastats"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "MetaStatsCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

MetaStatsCommand::MetaStatsCommand(string option) {
	try {
		abort = false; calledHelp = false;   
		allLines = true;
		
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			
			//check to make sure all parameters are valid for command
			map<string,string>::iterator it;
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["metastats"] = tempOutNames;
			
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("design");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["design"] = inputDir + it->second;		}
				}
				
				it = parameters.find("shared");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["shared"] = inputDir + it->second;		}
				}
				
                it = parameters.find("lcr");
                //user has given a template file
                if(it != parameters.end()){
                    path = util.hasPath(it->second);
                    //if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {    parameters["lcr"] = inputDir + it->second;        }
                }
			}
			
			//check for required parameters
			sharedfile = validParameter.validFile(parameters, "shared");
			if (sharedfile == "not open") { abort = true; }
            else if (sharedfile == "not found") { sharedfile = "";  }
			else { current->setSharedFile(sharedfile); inputfile = sharedfile; format = "sharedfile";  }
			
            lcrfile = validParameter.validFile(parameters, "lcr");
            if (lcrfile == "not open") { abort = true; }
            else if (lcrfile == "not found") { lcrfile = "";  }
            else {
                current->setLCRFile(lcrfile); inputfile = lcrfile; format = "lcrfile";
                m->mothurOut("[NOTE]: When using an lcr file mothur will run the fisher exact test with the floor of the values generated.\n");
            }
            
            if ((sharedfile == "") && (lcrfile == "")) {
                //is there are current file available for any of these?
                //give priority to shared, then list, then rabund, then sabund
                //if there is a current shared file, use it
                sharedfile = current->getSharedFile();
                if (sharedfile != "") { inputfile = sharedfile; format = "sharedfile"; m->mothurOut("Using " + sharedfile + " as input file for the shared parameter.\n"); }
                else {
                    lcrfile = current->getLCRFile();
                    if (lcrfile != "") { inputfile = lcrfile; format = "lcrfile"; m->mothurOut("Using " + lcrfile + " as input file for the lcr parameter.\n");  m->mothurOut("[NOTE]: When using an lcr file mothur will run the fisher exact test with the floor of the values generated.\n"); }
                    else { m->mothurOut("No valid current files. You must provide a lcrfile or shared file.\n"); abort = true; }
                }
            }
        
			//check for required parameters
			designfile = validParameter.validFile(parameters, "design");
			if (designfile == "not open") { abort = true; }
			else if (designfile == "not found") {  				
				//if there is a current design file, use it
				designfile = current->getDesignFile(); 
				if (designfile != "") { m->mothurOut("Using " + designfile + " as input file for the design parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current designfile and the design parameter is required."); m->mothurOutEndLine(); abort = true; }
			}else { current->setDesignFile(designfile); }
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	
				outputDir = "";	outputDir += util.hasPath(inputfile); //if user entered a file with a path then preserve it
			}

			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			label = validParameter.valid(parameters, "label");			
			if (label == "not found") { label = ""; }
			else { 
				if(label != "all") {  util.splitAtDash(label, labels);  allLines = false;  }
				else { allLines = true;  }
			}
			
			groups = validParameter.valid(parameters, "groups");			
			if (groups == "not found") { groups = ""; pickedGroups = false; }
			else { 
				pickedGroups = true;
				util.splitAtDash(groups, Groups);
                if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } }
            }
			
			sets = validParameter.valid(parameters, "sets");
			if (sets == "not found") { sets = ""; }
			else { 
				util.splitAtDash(sets, Sets);
                if (Sets.size() != 0) { if (Sets[0] != "all") { Groups.clear(); } }
			}
			
			string temp = validParameter.valid(parameters, "iters");			if (temp == "not found") { temp = "1000"; }
			util.mothurConvert(temp, iters); 
			
			temp = validParameter.valid(parameters, "threshold");			if (temp == "not found") { temp = "0.05"; }
			util.mothurConvert(temp, threshold); 
			
			temp = validParameter.valid(parameters, "processors");	if (temp == "not found"){	temp = current->getProcessors();	}
			processors = current->setProcessors(temp);
		}

	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "MetaStatsCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int MetaStatsCommand::execute(){
	try {
	
        if (abort) { if (calledHelp) { return 0; }  return 2;	}
        
		DesignMap* designMap = new DesignMap(designfile);

		InputData input(inputfile, format, Groups);
		set<string> processedLabels;
		set<string> userLabels = labels;
        string lastLabel = "";
        
        SharedRAbundVectors* lookup = NULL; SharedLCRVectors* lcr = NULL;
        
        if (format == "sharedfile") {
            lookup = util.getNextShared(input, allLines, userLabels, processedLabels, lastLabel);
            Groups = lookup->getNamesGroups();
        }else {
            lcr = util.getNextLCR(input, allLines, userLabels, processedLabels, lastLabel);
            Groups = lcr->getNamesGroups();
        }
        
        if (Sets.size() == 0) { Sets = designMap->getCategory();  }
		int numGroups = (int)Sets.size();
		for (int a=0; a<numGroups; a++) { 
			for (int l = 0; l < a; l++) {	
				vector<string> groups; groups.push_back(Sets[a]); groups.push_back(Sets[l]);
				namesOfGroupCombos.push_back(groups);
			}
		}
	
		if (numGroups == 2) { processors = 1; }
		else if (numGroups < 2)	{ m->mothurOut("[ERROR]: Not enough sets, I need at least 2 valid sets. Unable to complete command.\n");  m->setControl_pressed(true); }
        
        while ((lookup != NULL) || (lcr != NULL)){
            
            if (m->getControl_pressed()) { if (lookup != NULL) { delete lookup; } if (lcr != NULL) { delete lcr; }break; }
            
            if (format == "sharedfile") {
                process(lookup, designMap); delete lookup;
                lookup = util.getNextShared(input, allLines, userLabels, processedLabels, lastLabel);
            }
            else {
                process(lcr, designMap); delete lcr;
                lcr = util.getNextLCR(input, allLines, userLabels, processedLabels, lastLabel);
            }
        }
        
        delete designMap;
        if (m->getControl_pressed()) {  outputTypes.clear(); if (lookup != NULL) { delete lookup; } if (lcr != NULL) { delete lcr; } for (int i = 0; i < outputNames.size(); i++) {    util.mothurRemove(outputNames[i]); } return 0; }
		
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i] +"\n"); 	} m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "execute");
		exit(1);
	}
}
/**************************************************************************************************/
struct metastatsData {
    SharedRAbundVectors* thisLookUp;
    SharedLCRVectors* thisLCR;
    vector< vector<string> > namesOfGroupCombos;
    vector<string> designMapGroups, outputNames;
    int start, num, iters, count;
    float threshold;
    Utils util;
    MothurOut* m;
    
    metastatsData(){}
    metastatsData(int st, int en, vector<string> on, vector< vector<string> > ns, SharedRAbundVectors*& lu, vector<string> dg, int i, float thr) {
        m = MothurOut::getInstance();
        outputNames = on;
        start = st;
        num = en;
        namesOfGroupCombos = ns;
        thisLookUp = lu;
        designMapGroups = dg;
        iters = i;
        threshold = thr;
        count=0;
        thisLCR = NULL;
    }
    
    metastatsData(int st, int en, vector<string> on, vector< vector<string> > ns, SharedLCRVectors*& lu, vector<string> dg, int i, float thr) {
        m = MothurOut::getInstance();
        outputNames = on;
        start = st;
        num = en;
        namesOfGroupCombos = ns;
        thisLCR = lu;
        designMapGroups = dg;
        iters = i;
        threshold = thr;
        count=0;
        thisLookUp = NULL;
    }
};
//**********************************************************************************************************************
int driverShared(metastatsData* params) {
    try {
        
        vector<string> thisLookupNames = params->thisLookUp->getNamesGroups();
        vector<SharedRAbundVector*> thisLookupRabunds = params->thisLookUp->getSharedRAbundVectors();
        
        //for each combo
        for (int c = params->start; c < (params->start+params->num); c++) {
            
            //get set names
            string setA = params->namesOfGroupCombos[c][0];
            string setB = params->namesOfGroupCombos[c][1];
            string outputFileName = params->outputNames[c];
            
            vector< vector<double> > data2; data2.resize(params->thisLookUp->getNumBins());
            
            vector<SharedRAbundVector*> subset;
            vector<string> subsetGroups;
            int setACount = 0; int setBCount = 0;
            for (int i = 0; i < params->thisLookUp->size(); i++) {
                string thisGroup = thisLookupNames[i];
                if (params->designMapGroups[i] == setB) {
                    subset.push_back(thisLookupRabunds[i]);
                    subsetGroups.push_back(thisGroup);
                    setBCount++;
                }else if (params->designMapGroups[i] == setA) {
                    subset.insert(subset.begin()+setACount, thisLookupRabunds[i]);
                    subsetGroups.insert(subsetGroups.begin()+setACount, thisGroup);
                    setACount++;
                }
            }
            
            if ((setACount == 0) || (setBCount == 0))  { params->m->mothurOut("Missing shared info for " + setA + " or " + setB + ". Skipping comparison.\n"); }
            else {
                for (int j = 0; j < params->thisLookUp->getNumBins(); j++) {
                    data2[j].resize(subset.size(), 0.0);
                    for (int i = 0; i < subset.size(); i++) { data2[j][i] = (subset[i]->get(j)); }
                }
                
                params->m->mothurOut("\nComparing " + setA + " and " + setB + "...\n");
                MothurMetastats mothurMeta(params->threshold, params->iters);
                mothurMeta.runMetastats(outputFileName , data2, setACount, params->thisLookUp->getOTUNames(), true);
                params->m->mothurOutEndLine();
            }
        }
        
        for(int i = 0; i < thisLookupRabunds.size(); i++)  {  delete thisLookupRabunds[i];  }
        
        return 0;
        
    }
    catch(exception& e) {
        params->m->errorOut(e, "MetaStatsCommand", "driver");
        exit(1);
    }
}
//**********************************************************************************************************************
int MetaStatsCommand::process(SharedRAbundVectors*& thisLookUp, DesignMap*& designMap){
	try {
        vector<linePair> lines;
        vector<std::thread*> workerThreads;
        vector<metastatsData*> data;
        
        int remainingPairs = namesOfGroupCombos.size();
        int startIndex = 0;
        vector<vector<string> > outputFileNames;
        for (int remainingProcessors = processors; remainingProcessors > 0; remainingProcessors--) {
            int numPairs = remainingPairs; //case for last processor
            if (remainingProcessors != 1) { numPairs = ceil(remainingPairs / remainingProcessors); }
            lines.push_back(linePair(startIndex, numPairs)); //startIndex, numPairs
            
            for (int i = startIndex; i < startIndex+numPairs; i++) {
                //get set names
                string setA = namesOfGroupCombos[i][0];
                string setB = namesOfGroupCombos[i][1];
                
                //get filename
                map<string, string> variables;
                variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                variables["[distance]"] = thisLookUp->getLabel();
                variables["[group]"] = setA + "-" + setB;
                string outputFileName = getOutputFileName("metastats",variables);
                outputNames.push_back(outputFileName); outputTypes["metastats"].push_back(outputFileName);
            }
            
            startIndex = startIndex + numPairs;
            remainingPairs = remainingPairs - numPairs;
        }
        
        vector<string> designMapGroups = thisLookUp->getNamesGroups();
        for (int j = 0; j < designMapGroups.size(); j++) {  designMapGroups[j] = designMap->get(designMapGroups[j]); }
        
        //Lauch worker threads
        for (int i = 0; i < processors-1; i++) {
            //make copy of lookup so we don't get access violations
            SharedRAbundVectors* newLookup = new SharedRAbundVectors(*thisLookUp);
            
            metastatsData* dataBundle = new metastatsData(lines[i+1].start, lines[i+1].end, outputNames, namesOfGroupCombos, newLookup, designMapGroups, iters, threshold);
            data.push_back(dataBundle);
            
            std::thread* thisThread = new std::thread(driverShared, dataBundle);
            workerThreads.push_back(thisThread);
        }

        metastatsData* dataBundle = new metastatsData(lines[0].start, lines[0].end, outputNames, namesOfGroupCombos, thisLookUp, designMapGroups, iters, threshold);
        driverShared(dataBundle);
        
        for (int i = 0; i < processors-1; i++) {
            workerThreads[i]->join();
            
            delete data[i]->thisLookUp;
            delete data[i];
            delete workerThreads[i];
        }
        delete dataBundle;
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "MetaStatsCommand", "process");
		exit(1);
	}
}
//**********************************************************************************************************************
int driverLCR(metastatsData* params) {
    try {
        
        vector<string> thisLookupNames = params->thisLCR->getNamesGroups();
        vector<SharedLCRVector*> thisLCRRabunds = params->thisLCR->getSharedLCRVectors();
        
        //for each combo
        for (int c = params->start; c < (params->start+params->num); c++) {
            
            //get set names
            string setA = params->namesOfGroupCombos[c][0];
            string setB = params->namesOfGroupCombos[c][1];
            string outputFileName = params->outputNames[c];
            
            vector< vector<double> > data2; data2.resize(params->thisLCR->getNumBins());
            
            vector<SharedLCRVector*> subset;
            int setACount = 0; int setBCount = 0;
            for (int i = 0; i < params->thisLCR->size(); i++) {
                string thisGroup = thisLookupNames[i];
                if (params->designMapGroups[i] == setB) {
                    subset.push_back(thisLCRRabunds[i]);
                    setBCount++;
                }else if (params->designMapGroups[i] == setA) {
                    subset.insert(subset.begin()+setACount, thisLCRRabunds[i]);
                    setACount++;
                }
            }
            
            if ((setACount == 0) || (setBCount == 0))  { params->m->mothurOut("Missing shared info for " + setA + " or " + setB + ". Skipping comparison.\n"); }
            else {
                for (int j = 0; j < params->thisLCR->getNumBins(); j++) {
                    data2[j].resize(subset.size(), 0.0);
                    for (int i = 0; i < subset.size(); i++) { data2[j][i] = (subset[i]->get(j)); }
                }
                
                params->m->mothurOut("\nComparing " + setA + " and " + setB + "...\n");
                MothurMetastats mothurMeta(params->threshold, params->iters);
                mothurMeta.runMetastats(outputFileName , data2, setACount, params->thisLCR->getOTUNames(), false); 
                params->m->mothurOutEndLine();
            }
        }
        
        for(int i = 0; i < thisLCRRabunds.size(); i++)  {  delete thisLCRRabunds[i];  }
        
        return 0;
        
    }
    catch(exception& e) {
        params->m->errorOut(e, "MetaStatsCommand", "driver");
        exit(1);
    }
}
//**********************************************************************************************************************
int MetaStatsCommand::process(SharedLCRVectors*& thisLCR, DesignMap*& designMap){
    try {
        vector<linePair> lines;
        vector<std::thread*> workerThreads;
        vector<metastatsData*> data;
        
        int remainingPairs = namesOfGroupCombos.size();
        int startIndex = 0;
        vector<vector<string> > outputFileNames;
        for (int remainingProcessors = processors; remainingProcessors > 0; remainingProcessors--) {
            int numPairs = remainingPairs; //case for last processor
            if (remainingProcessors != 1) { numPairs = ceil(remainingPairs / remainingProcessors); }
            lines.push_back(linePair(startIndex, numPairs)); //startIndex, numPairs
            
            for (int i = startIndex; i < startIndex+numPairs; i++) {
                //get set names
                string setA = namesOfGroupCombos[i][0];
                string setB = namesOfGroupCombos[i][1];
                
                //get filename
                map<string, string> variables;
                variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                variables["[distance]"] = thisLCR->getLabel();
                variables["[group]"] = setA + "-" + setB;
                string outputFileName = getOutputFileName("metastats",variables);
                outputNames.push_back(outputFileName); outputTypes["metastats"].push_back(outputFileName);
            }
            
            startIndex = startIndex + numPairs;
            remainingPairs = remainingPairs - numPairs;
        }
        
        vector<string> designMapGroups = thisLCR->getNamesGroups();
        for (int j = 0; j < designMapGroups.size(); j++) {  designMapGroups[j] = designMap->get(designMapGroups[j]); }
        
        //Lauch worker threads
        for (int i = 0; i < processors-1; i++) {
            //make copy of lookup so we don't get access violations
            SharedLCRVectors* newLookup = new SharedLCRVectors(*thisLCR);
            
            metastatsData* dataBundle = new metastatsData(lines[i+1].start, lines[i+1].end, outputNames, namesOfGroupCombos, newLookup, designMapGroups, iters, threshold);
            data.push_back(dataBundle);
            
            std::thread* thisThread = new std::thread(driverLCR, dataBundle);
            workerThreads.push_back(thisThread);
        }

        metastatsData* dataBundle = new metastatsData(lines[0].start, lines[0].end, outputNames, namesOfGroupCombos, thisLCR, designMapGroups, iters, threshold);
        driverLCR(dataBundle);
        
        for (int i = 0; i < processors-1; i++) {
            workerThreads[i]->join();
            
            delete data[i]->thisLookUp;
            delete data[i];
            delete workerThreads[i];
        }
        delete dataBundle;
        return 0;
    }
    catch(exception& e) {
        m->errorOut(e, "MetaStatsCommand", "process");
        exit(1);
    }
}
//**********************************************************************************************************************/


