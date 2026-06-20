#!/usr/bin/Rscript

args <- commandArgs(trailingOnly = TRUE)
InMatFile <-args[1]


if (length(args) != 1) 
{
	cat("\t\tUsage: Rscript vistreecode.r  p_dis.mat \n")
	cat("\t\tPlease provide the input matrix file p_dis.mat \n")
	q("no")
}


if  (!file.exists(InMatFile))
{
	cat("\t\tInPut p_dis.mat is no exists,please check\n")
	q("no")
}



if(!require(ape)) install.packages("ape")
library(ape)

# read data from file 
matrix_data <- read.table(InMatFile, header=F, row.names=1,skip=1)
dist_matrix <- as.dist(as.matrix(matrix_data))

# Construct a tree using UPGMA/NJ method
# tree <-  as.phylo(hclust(dist_matrix, method = "average"))   # your can try UPGMA
tree <- nj(dist_matrix)
# print tree
#pdf("treeA.pdf") ;print(tree); dev.off();

# save the tree to Newick format file
write.tree(tree, file="output_tree.nwk") 

if (!require("BiocManager", quietly = TRUE))  install.packages("BiocManager")
if(!require(ggtree)) BiocManager.packages("ggtree")	
library(ggtree)
if(!require(dplyr))  BiocManager::install("dplyr")
library(dplyr)
sample_groups <- rep("UnKnow", length(tree$tip.label))
pop_info <- read.table("sample.group", header = FALSE, stringsAsFactors = FALSE, col.names = c("label", "group"))
match_indices <- match(tree$tip.label, pop_info$label)
sample_groups[!is.na(match_indices)] <- pop_info$group[match_indices[!is.na(match_indices)]]
group_info <- data.frame(label = tree$tip.label, group = sample_groups)
tree<-full_join(tree,group_info,by="label")
tree@extraInfo$group[is.na(tree@extraInfo$group)]<-"ZZBranches"
g <- ggtree(tree,layout="equal_angle",aes(color = group))
#g <- ggtree(tree,layout="equal_angle")  # see more at ggtree
# save tree pdf
ggsave("tree_visualization.pdf", g)


