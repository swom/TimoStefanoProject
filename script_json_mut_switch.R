library(rjson)
library(ggplot2)
library(tibble)
library(dplyr)

Timetochangecalc = function (threshold,lbound, ubound, simple_res){
  curenv=0
  timing=FALSE
  stor=vector()
  for (i in lbound:ubound){
    
    if (curenv!=simple_res$m_env_values[i]) {
      curenv=simple_res$m_env_values[i]
      timing=TRUE
      elapsed=0
    }
    
    if (timing == TRUE){
      elapsed=elapsed+1
      if (simple_res$m_avg_fitnesses[i] > threshold){
        timing=FALSE
        stor=append(stor, elapsed)
        
      }
    }  
    
  }
  
  return (stor)
}

############################################# end of preamble ###############


setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/Linear")
setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/Sigmoid (stefano)")

results <- fromJSON(file = "sim1-10-2-1-1.json")
simple_res = rowid_to_column(as_tibble(results[c(1,2,4)]), var = "gen")

ggplot(data = simple_res %>% slice_max(gen,n = 1000))+
  geom_line(aes(x = gen, y = m_avg_fitnesses)) +
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1.5,
                fill = as.factor(m_env_values),
                alpha = 0.5))



output=Timetochangecalc(0.9, 1, 100000, simple_res)


barplot (output, xlab = "Nr environmental change (gen 1-1000000)", ylab = "Time to adaptation (in generations)")





dev.off()





