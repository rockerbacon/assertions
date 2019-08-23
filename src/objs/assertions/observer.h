#pragma once

#include <sstream>
#include "assertions/stopwatch.h"
#include <list>
#include <fstream>

namespace benchmark {

	class ObservableVariable {
		public:
			virtual ~ObservableVariable(void) = default;
			virtual std::string getLabel (void) = 0;
			virtual std::string getValue (void) = 0;
	};

	template<typename T>
	class ObservableVariableTemplate : public ObservableVariable {
		private:
			const std::string label;
			const T& variable;
		public:
			ObservableVariableTemplate (const std::string& label, const T& variable) : variable(variable), label(label) {}

			std::string getLabel (void) {
				return this->label;
			}

			std::string getValue (void) {
				std::ostringstream strBuilder;
				strBuilder << this->variable;
				return strBuilder.str();
			}

	};

	class Observer {
		public:
			virtual ~Observer (void) = default;
			virtual void notifyBenchmarkBegun (const std::string& benchmarkTitle, unsigned numberOfRuns) = 0;
			virtual void notifyRunBegun (void) = 0;
			virtual void notifyRunEnded (void) = 0;
			virtual void notifyBenchmarkEnded (void) = 0;
	};

	class TerminalObserver : public Observer {
		private:
			Stopwatch stopwatch;
			std::list<ObservableVariable*> variablesToObserve;
			unsigned numberOfRuns;
		public:
			TerminalObserver (void) = default;
			~TerminalObserver (void);

			template<typename T>
			void observeVariable (const std::string& variableLabel, const T& variable) {
				ObservableVariableTemplate<T> *observableVariable = new ObservableVariableTemplate<T>(variableLabel, variable);
				this->variablesToObserve.push_back(observableVariable);
			}

			void notifyBenchmarkBegun (const std::string& benchmarkTitle, unsigned numberOfRuns);
			void notifyRunBegun (void);
			void notifyRunEnded (void);
			void notifyBenchmarkEnded (void);
	};

	class TextFileObserver : public Observer {
		private:
			std::ofstream outputFile;
			std::list<ObservableVariable*> variablesToObserve;
			unsigned numberOfRuns;
		public:
			TextFileObserver (const std::string& outputFilePath);
			~TextFileObserver (void);

			template<typename T>
			void observeVariable (const std::string& variableLabel, const T& variable) {
				ObservableVariableTemplate<T> *observableVariable = new ObservableVariableTemplate<T>(variableLabel, variable);
				this->variablesToObserve.push_back(observableVariable);
			}

			void notifyBenchmarkBegun (const std::string &benchmarkTitle, unsigned numberOfRuns);
			void notifyRunBegun (void);
			void notifyRunEnded (void);
			void notifyBenchmarkEnded (void);

	};
}
