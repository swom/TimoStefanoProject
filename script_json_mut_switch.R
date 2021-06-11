library(rjson)
library(ggplot2)
library(tibble)

setwd("X:/build-mutational_switches-Desktop_Qt_6_0_0_MinGW_64_bit-Release")
results <- fromJSON(file = "sim.json")

simple_res = rowid_to_column(as_tibble(results[c(1,2,4)]), var = "gen")

ggplot(data = simple_res %>% slice_max(gen,n = 1000))+
  geom_line(aes(x = gen, y = m_avg_fitnesses)) +
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1.5,
                fill = as.factor(m_env_values),
                alpha = 0.5))

