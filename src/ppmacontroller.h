#ifndef PPMA_CONTROLLER_H
#define PPMA_CONTROLLER_H

#include <vector>
#include <map>

class PPMAController {
private:
    // Data structure to store communication patterns
    struct CommunicationPattern {
        int destination;
        double frequency;
        double lastTimestamp;
    };

    // Map of source to communication patterns
    std::map<int, std::vector<CommunicationPattern>> patternDatabase;

    // Learning rate for updating pattern frequencies
    double learningRate;

public:
    PPMAController(double rate = 0.1) : learningRate(rate) {}

    // Function to update communication patterns
    void updatePattern(int source, int destination, double timestamp) {
        // Check if the source has existing patterns
        if (patternDatabase.find(source) == patternDatabase.end()) {
            // If not, create a new entry
            patternDatabase[source] = std::vector<CommunicationPattern>();
        }

        std::vector<CommunicationPattern>& patterns = patternDatabase[source];
        bool found = false;

        // Check if the destination already exists in the patterns
        for (auto& pattern : patterns) {
            if (pattern.destination == destination) {
                // Update frequency and timestamp
                pattern.frequency = (1 - learningRate) * pattern.frequency + learningRate;
                pattern.lastTimestamp = timestamp;
                found = true;
                break;
            }
        }

        // If the destination is new, add it to the patterns
        if (!found) {
            CommunicationPattern newPattern = {destination, learningRate, timestamp};
            patterns.push_back(newPattern);
        }

        // Normalize frequencies to ensure they sum up to 1
        normalizeFrequencies(patterns);
    }

    // Function to predict the next destination
    int predictNextDestination(int source) {
        if (patternDatabase.find(source) == patternDatabase.end()) {
            return -1; // No pattern found
        }

        const std::vector<CommunicationPattern>& patterns = patternDatabase[source];
        if (patterns.empty()) {
            return -1; // No pattern found
        }

        // Select the destination with the highest frequency
        int predictedDestination = -1;
        double highestFrequency = 0.0;

        for (const auto& pattern : patterns) {
            if (pattern.frequency > highestFrequency) {
                highestFrequency = pattern.frequency;
                predictedDestination = pattern.destination;
            }
        }

        return predictedDestination;
    }

private:
    // Helper function to normalize frequencies
    void normalizeFrequencies(std::vector<CommunicationPattern>& patterns) {
        double sum = 0.0;
        for (const auto& pattern : patterns) {
            sum += pattern.frequency;
        }

        if (sum > 0.0) {
            for (auto& pattern : patterns) {
                pattern.frequency /= sum;
            }
        }
    }
};

#endif
