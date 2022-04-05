library(rjson)
library(data.table)
library(tidyverse)
library(ggplot2)
library(stringi)
library(rlist)
library(igraph)
library(ggraph)
library(tidygraph)
library(ggnetwork)
library(networkD3)
library(magick)

dir = dirname(rstudioapi::getActiveDocumentContext()$path)
setwd(dir)

results=list()
# pattern = "*json$"
pattern = "1-2-1_3_sigmoid_hifi.json"
i = pattern
for (i in  list.files(path = '.', pattern = pattern)){
  ###Making a data tibble with all top individuals' data 
  results <- fromJSON(file = i)
  names(results$m_top_inds) = as.numeric(
      seq(from=0, by=1, 
          length.out=length(results$m_top_inds)
          )
      )
  results_unlist = results$m_top_inds %>% 
  unlist(recursive=FALSE)#%>%
  
  results_df = as_tibble(results_unlist) %>%
    filter(!row_number() %in% 2) %>% 
  add_column(var = c("fitness", "network"))%>%
    pivot_longer(!var, names_to = "ind_ID") %>%
    pivot_wider(names_from = var, values_from = value)  %>%
    mutate(ind_ID = as.numeric(ind_ID)) %>% 
    mutate(ind_ID = format(ind_ID, scientific = FALSE)) #%>% 
    #add generation as grouping variable
  #   mutate(gen = as.numeric(ifelse(as.numeric(ind_ID) %% 10 != 0,
  #                       substr(ind_ID, 0, nchar(ind_ID) - 1),
  #                       substr(ind_ID, 0, nchar(ind_ID) - 2))))%>% 
  #giving inds ID
  # mutate(ind_ID = substr(ind_ID, nchar(ind_ID) - 1, nchar(ind_ID)))
  
   #create ID from title of file
  ID = data.frame(i) %>% 
    separate(i, c("architecture","seed","activation_func"), sep = '_')%>% 
    separate(seed, c("seed",NA))
  
  ID$architecture = as.factor(ID$architecture)
  ID$seed = as.factor(ID$seed)
  ID$activation_func = as.factor(ID$activation_func) %>% str_replace( ".json", "")
  
  name1 = paste("top_inds", str_replace(i, ".json", ""), sep = "_")
  assign(name1, cbind(results_df, ID))
  
  ###Making a number vector out of the architecture
  architecture = strsplit(levels(get(name1)$architecture)[1], "-")[[1]]%>%
    as.integer()
  
  
  ###Keeping only network architecture,
  #  expanding to have each connection as a row
  
  top_inds_net = unnest_wider(get(name1), col = "network")%>%
        mutate(m_input_size = NULL, fitness = NULL)%>%
    unnest_wider(col = "m_network_weights", names_sep = "_layer_")%>%
    #need to make list of nodes for first layer
    #as fromJSON does not recognize 1-elemnt vectors
    mutate(m_network_weights_layer_1 = lapply(m_network_weights_layer_1, function(x) as.list(x))) %>% 
    #
    pivot_longer(cols = sprintf("m_network_weights_layer_%s", seq(1:(length(architecture)-1))),
                 names_to = "layer")%>%
    unnest_wider(col = "value", names_sep = "_node_")%>%
    #make weights of nodes arrays
    #since fromJSON does not recognize 1 element arrays
    mutate(value_node_1 = lapply(value_node_1, function(x) as.array(x))) %>% 
    mutate(value_node_2 = lapply(value_node_2, function(x) as.array(x))) %>% 
    pivot_longer(cols = sprintf("value_node_%s", seq(1:(max(architecture)))),
                 names_to = "node")%>%
    drop_na()%>%
    unnest_wider(col = "value", names_sep = "_weight_")%>%
    pivot_longer(cols = sprintf("value_weight_%s", seq(1:(max(architecture)))),
                 names_to = "weight")%>%
    drop_na()%>%
    mutate(w_sign = if_else(value < 0, 1, 2))
  
  top_inds_net$gen = as.factor(top_inds_net$gen)
  top_inds_net$w_sign = as.factor(top_inds_net$w_sign)
  
  name2 = paste("top_inds_net", str_replace(i, ".json", ""), sep = "_")
  assign(name2, top_inds_net)
  
  #generate coordinates for positioning nodes correctly in plot
  x = vector()
  y = vector()
  for(j in 1:length(architecture)){
    x = c(x, rep(j, architecture[j]))
    y = c(y, seq(1:architecture[j]) + ((max(architecture)-architecture[j])/2))
  }
  
  l =   cbind(x, y)
  
  #Make the list of nodes
  layer = vector()
  node = vector()
  for(j in 1:length(architecture)){
    layer = c(layer, rep(j, architecture[j]))
    node = c(node, seq(1:architecture[j]))
  }
  id = seq(1:length(layer))
  
  node_tibble = as_tibble(cbind(id, layer, node)) %>% 
    mutate(id = as.factor(id))
  
  #Make the list of edges
  
  from = vector()
  to = vector()
  for(j in 1:nrow(node_tibble)){
    from = c(from, rep(node_tibble$id[j],
                       nrow(filter(node_tibble, layer == (node_tibble$layer[j]+1)))))
    to = c(to, filter(node_tibble, layer == (node_tibble$layer[j]+1))$id)
  }
  edge_tibble = as_tibble(cbind(from, to))
  

  #####Now let's loop through generations####
  # for(j in levels(get(name2)$gen)){
  for(j in levels(get(name2)$ind_ID)){
      #j = "994000"
  print(j)
    
      #adding weights, weight sign, activation to the edge list
    # ind = filter(get(name2), gen == j, ind_ID == "01")
    ind = filter(get(name2), ind_ID == j)
    edge_tibble_ind = cbind(edge_tibble, ind$value, ind$w_sign)
  
  node_tibble = as_tibble(cbind(id, layer, node))

  # ###plot network####
  ##create igraph or ggraph object
  network_d <- igraph::graph_from_data_frame(d = edge_tibble_ind,
                                             vertices = node_tibble,
                                             directed = T)
  
  E(network_d)$color = as.factor(edge_tibble_ind$`ind$w_sign`)
  E(network_d)$value = edge_tibble_ind$`ind$value`
  # network_d = network_d - E(network_d)[E(network_d)$weight == 0]


  jpeg(paste("Plot",
             "s", ind$seed,
             "arch",ind$architecture,
             "cycle", 
             paste(rep("0",max(nchar(levels(ind$ind_ID)))-(nchar(j))),j, sep=""),
             ".png",
             sep = "_")
       ,width = 700,
       height = 700)
  
  title = paste("Generation ", ind$ind_ID[1])

  plot(network_d, layout = l,
       size = abs(V(network_d)$value/max(V(network_d)$value) * 10), 
       edge.arrow.size = 0.5,                           # Arrow size, defaults to 1
       edge.arrow.width = 0.7,                          # Arrow width, defaults to 1
       edge.arrow.height = 0.9,                          # Arrow width, defaults to 1
       edge.lty = c("solid"),
       edge.width = abs(E(network_d)$value/max(E(network_d)$value) * 10), 
       main = title,
       vertex.label = T
       )
  dev.off()
  
  }  
 
}
#####Create gif #####
  ###list file names and read in
  imgs = intersect(intersect(intersect(list.files(pattern = "*png$", full.names = T), list.files(pattern = levels(get(name1)$architecture)[1], full.names =  T)),
                   list.files(pattern = levels(get(name1)$mut_rate_dup)[1], full.names =  T)),
                   list.files(pattern = levels(get(name1)$change_freq)[1], full.names =  T)
                   ) 
  img_list = lapply(imgs, image_read)
  
  ## join the images together
  img_joined <- image_join(img_list)
  img_sub = head(img_joined, length(imgs) / 10)
  ## animate at 2 frames per second
  img_animated <- image_animate(img_sub, fps = 2)
  
  ## save to disk

  path = paste("Gif",get(name1)$seed[1], get(name1)$mut_rate_dup[1], get(name1)$change_freq[1], get(name1)$architecture[1], ".gif", sep = "_")
  image_write(image = img_animated,
              path = path)
  
