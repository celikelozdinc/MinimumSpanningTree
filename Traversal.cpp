/*
* Implements Kruskal algorithm
* for obtaining least-cost spanning tree
* All the repository can be found on this repository :
* github.com/celikelozdinc/MinimumSpanningTree
*
*   Edge
*       Represents the edge in the graph along with its source and destination nodes and it's cost/weight
*       Weights are stored in undirected manner
*   PathFinder
*       Intermediate structure for traversing all the nodes in the graph
*       Sorts all the edges with respect to their costs/weights and then
*       Searches all the edges in order to find all the optimum edges
*           for ensuring all the nodes in the graph is traversed
*       Once an edge is inserted, tries to combine different edges for augmenting graph traversal
*           For example, if edge (a,b) is inserted and (b,y) is already in the traversed list,
*               augment traversed list with the node (a,y)
*   SpanningTree
*       Actual structure for constructing spanning trees
*       Calculates cost of the spanning tree
*           once an edge is inserted
*/

#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

const string INPUT_FILE = "mst_data.in";

class Edge{
    public:
        Edge(const int s=-1, const int d=-1, const int w=-1) : source(s), destination(d), weight(w) {} // INITIALIZER LIST SYNTAX
        int get_source() {return this->source ;}
        int get_destination() {return this->destination ;}
        int get_weight() {return this->weight ;}
        bool operator==(const Edge e);
    private:
        int weight, source,destination;
};

bool Edge::operator==(const Edge e) {
    return ( (this->source == e.source) && (this->destination == e.destination) && (this->weight == e.weight) ) ;
}

class PathFinder{
    public:
        PathFinder() : index(0),num_of_traversed_edges(0) {} //INITIALIZER LIST SYNTAX
        Edge* traverse() ;
        void parse_input(const string input_file);
        void insert_new_edge(const int s, const int d, const int w);
        void sort_edges(){sort(this->all_weights.begin(),this->all_weights.end());} // Performs Quick Sort
        void print() ;
        int get_node_size() {return this->num_of_nodes;}
        bool are_nodes_traversed(const int source, const int destination);
        bool is_edge_traversed(const int source, const int destination);
        bool check_cycle(const int source, const int destination);
        void augment_components(const int source, const int destination);
    private:
        vector<pair<int,int>> all_weights;
        vector<Edge> all_edges;
        set<pair<int,int>> traversed_edges;
        int index, num_of_nodes, num_of_traversed_edges;
        set<int> traversed_nodes;
};

bool PathFinder::are_nodes_traversed(const int source, const int destination) {
    bool is_source_found = false, is_destination_found = false;
    for (auto &traversed_node : this->traversed_edges ) {
        if (traversed_node.first == source ) {
            is_source_found = true;
        }

        if (traversed_node.second == destination) {
            is_destination_found = true;
        }
    }
    return (is_source_found and is_destination_found);
}

bool PathFinder::is_edge_traversed(const int source, const int destination) {
    bool is_edge_traversed = false;
    if (find(this->traversed_edges.begin(),this->traversed_edges.end(),make_pair(source,destination)) != this->traversed_edges.end()) {
        is_edge_traversed = true;
    }

    return is_edge_traversed;
}

void PathFinder::parse_input(const string input_file){
    ifstream input(input_file);
    // Read first line indicating node size of graph
    int nodes;
    if (input >> nodes) {
        this -> num_of_nodes = nodes;
    }

    // Read source node, destination node and weight of the edge between these nodes
    int source,destination,weight;
    while(input >> source >> destination >> weight) {
        this->insert_new_edge(source,destination,weight);
    }
}

void PathFinder::insert_new_edge(const int s, const int d, const int w) {
    // Stores tuple of i,j,weight corresponding edge

    // insert if a new edge comes
    // if (x,y,w) already exist, do not add (y,x,w) again
    if ( find(this->all_edges.begin(),this->all_edges.end(),Edge(d,s,w)) != this->all_edges.end() ){
        cout<< "Can not insert edge (" << s << "," << d << "). Since there exist another edge (" 
        <<   d << "," << s << ") in the graph" << endl; 
        return;
    }
    
    // Add this new-coming edge
    this->all_edges.push_back(Edge(s,d,w));

    // Store weight of the edge along with its index for sorting in convenient way
    this->all_weights.push_back(make_pair(w,this->index));
    
    // Increment id of the edge
    this->index ++;
}

void PathFinder::print() {
    for (auto &item : this->all_weights) {
        //cout << "Index : " << item.second <<", Weight: " << item.first<< endl;
        cout << "Edge[" << item.second <<"] => Source : " << this->all_edges[item.second].get_source() 
            << ", Destination: " << this->all_edges[item.second].get_destination() 
            << ", Weight: " << this->all_edges[item.second].get_weight()
            << endl;
    }
}

bool PathFinder::check_cycle(const int source, const int destination) {
    cout << "Will check cycles for (" << source << "," << destination << ")" << endl;
    // Let new item be (x,t)
    // If traversed vector contains both (x,y) && (t,y), we'll see a loop here
    bool is_found = false;
    for (int idx = 0; idx < num_of_nodes; ++idx) {
        if(  (is_edge_traversed(source,idx)) &&  (is_edge_traversed(destination,idx))   ) {
            cout << "CYCLE exist regarding (" << source << "," << idx << ")" " and " <<
                "(" << destination << "," << idx << ")" <<endl;
            is_found = true;
        } 
    }
    

    // Let new item be (y,z)
    // If traversed vector contains both (x,y) && (x,z), we'll see a loop here
    for (int idx = 0; idx < num_of_nodes; ++idx) {
        if(  (is_edge_traversed(idx,source)) &&  (is_edge_traversed(idx,destination))   ) {
            cout << "CYCLE exist regarding (" << idx << "," << source << ")" " and " <<
                "(" << idx << "," << destination << ")" <<endl;
            is_found = true;
        } 
    }

    return is_found;
}

Edge* PathFinder::traverse() {
    int source_node, destination_node;
    bool is_source_traversed, is_destination_traversed;
    //sorted by item.first indicating weight of the edge
    for (auto &item : this->all_weights) {
        source_node = this->all_edges[item.second].get_source();
        destination_node = this->all_edges[item.second].get_destination();

        cout << "Processing edge (" << source_node << "," << destination_node << ") with weight " << item.first << endl;

        // If all nodes are traversed, which means containing node-1 edges, termination conditition
        if (this->num_of_traversed_edges == (this->num_of_nodes -1)) {
            cout << "Spanning tree now contains " << this->num_of_traversed_edges << " edges. Terminating..." << endl;
            return nullptr;
        }

        
        /*
        // If all nodes are traversed, terminate
        if (this->traversed_nodes.size () == (this->num_of_nodes )) {
            cout << "Spanning tree now contains " << this->traversed_nodes.size() << " nodes. Terminating..." << endl;
            return nullptr;
        }
        */
        

        // If this edge are traversed beforehand, skip this edge
        //if( are_nodes_traversed(source_node, destination_node)) {
        if (is_edge_traversed(source_node, destination_node)) {
            cout << "Edge (" << source_node << "," << destination_node << ") is already traversed. Skipping..." <<endl;
            continue;
        }

        // Do not create cycle
        if (check_cycle(source_node, destination_node)) {
            cout << "Edge (" << source_node << "," << destination_node << ") will create a loop. Skipping..." <<endl;
            // Erase current item from the traversal list
            this->all_weights.erase(remove(this->all_weights.begin(),this->all_weights.end(),item));
            return new Edge(-1,-1,-1);
        }

        // Mark source and destionation nodes as traversed, mark this edge as traversed
        this->traversed_nodes.insert(source_node);this->traversed_nodes.insert(destination_node);
        this->traversed_edges.insert(make_pair(source_node, destination_node));
        this->num_of_traversed_edges ++ ; // do not increase this counter while augmenting
        

        // Produce new nodes corresponding new node
        this->augment_components(source_node, destination_node);
        
        return &this->all_edges[item.second];
    }
}

void PathFinder::augment_components(const int source, const int destination) {
    for (  auto &edge : this->traversed_edges) {
        if ( (edge.second == source) && (edge.first != destination) )  {
            cout << "+++++ Augment traversed nodes with : (" << edge.first << "," << destination << ") +++++" << endl;
            // Mark source and destionation nodes as traversed, mark this edge as traversed
            this->traversed_nodes.insert(source);this->traversed_nodes.insert(destination);
            this->traversed_edges.insert(make_pair(edge.first,destination));
        }

        if ( (edge.first == destination) && (edge.second != source) ) {
            cout << "+++++ Augment traversed node with : (" << source << "," << edge.second << ") +++++" << endl;
            // Mark source and destionation nodes as traversed, mark this edge as traversed
            this->traversed_nodes.insert(source);this->traversed_nodes.insert(destination);
            this->traversed_edges.insert(make_pair(source,edge.second));
        }
        
    }
}

class SpanningTree {
    public:
        SpanningTree() : mst_cost(0) {}
        void add_edge(Edge* edge);
        void print();
    private:
        int mst_cost;
        vector<Edge> traversed_edges;
};

void SpanningTree::add_edge(Edge* edge) {
    // Mark this adge as traversed
    this->traversed_edges.push_back(*edge);

    // Total weight of spanning tree
    this->mst_cost += edge->get_weight();
}

void SpanningTree::print(){
    for(auto &edge : this->traversed_edges) {
        cout << "From " << edge.get_source() << 
            ", To: " << edge.get_destination() <<   
            ", Cost: " << edge.get_weight() << endl;
    }
    cout << "Cost of the Spanning Tree : " << this->mst_cost << endl; 
}

int main(void) {
    PathFinder pf;
    pf.parse_input(INPUT_FILE);
    pf.sort_edges();


    SpanningTree mst;
    Edge* new_edge;

    new_edge = pf.traverse();
    cout << "_____" << endl;
    while (new_edge != nullptr) {
    
    // means that, edge creates cycle, do not add to spanning tree
    if  ( ! ( (new_edge->get_destination() == -1) && (new_edge->get_source() == -1) && (new_edge->get_weight() == -1) ) ){
        mst.add_edge(new_edge); 
    }
    
    new_edge = pf.traverse();
    cout << "_____" << endl;
    }
    
    cout << "Minimum Spanning Tree and its components: " << endl;
    mst.print();

    return 0;
}